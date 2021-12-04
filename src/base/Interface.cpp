#include <simpledbus/base/Interface.h>

using namespace SimpleDBus;

Interface::Interface(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
                               const std::string& interface_name)
    : _conn(conn), _bus_name(bus_name), _path(path), _interface_name(interface_name), _loaded(true) {}

// ----- LIFE CYCLE -----

void Interface::load(Holder options) { _loaded = true; }

void Interface::unload() { _loaded = false; }

bool Interface::is_loaded() const { return _loaded; }

// ----- METHODS -----

Message Interface::create_method_call(const std::string& method_name) {
    return Message::create_method_call(_bus_name, _path, _interface_name, method_name);
}

// ----- PROPERTIES -----

void Interface::signal_property_changed(Holder changed_properties, Holder invalidated_properties) {}

Holder Interface::property_get_all() {
    Message query_msg = Message::create_method_call(_bus_name, _path, _interface_name, "GetAll");

    Holder h_interface = Holder::create_string(_interface_name);
    query_msg.append_argument(h_interface, "s");

    Message reply_msg = _conn->send_with_reply_and_block(query_msg);
    Holder result = reply_msg.extract();
    return result;
}

Holder Interface::property_get(const std::string& property_name) {
    Message query_msg = Message::create_method_call(_bus_name, _path, _interface_name, "Get");

    Holder h_interface = Holder::create_string(_interface_name);
    query_msg.append_argument(h_interface, "s");

    Holder h_name = Holder::create_string(property_name);
    query_msg.append_argument(h_name, "s");

    Message reply_msg = _conn->send_with_reply_and_block(query_msg);
    Holder result = reply_msg.extract();
    return result;
}

void Interface::property_set(const std::string& property_name, const Holder& value) {
    Message query_msg = Message::create_method_call(_bus_name, _path, _interface_name, "Set");

    Holder h_interface = Holder::create_string(_interface_name);
    query_msg.append_argument(h_interface, "s");

    Holder h_name = Holder::create_string(property_name);
    query_msg.append_argument(h_name, "s");

    query_msg.append_argument(value, "v");

    _conn->send_with_reply_and_block(query_msg);
}
