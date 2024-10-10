#include <iostream>
#include <memory>
#include <type_traits>

namespace Network {
    template<typename TPacket>
    struct IPacketHandler {};

/*
    template<typename TPlugin>
    struct ConceptPacket {
        using type = typename TPlugin::packet_type; // Adjust this to the actual nested type in TPlugin
    };
*/

    template<typename TPacket>
	concept ConceptPacket = std::is_trivial_v<TPacket>;

/*
    template<typename TPlugin, typename = void>
    void RegisterPacketHandler(std::shared_ptr<TPlugin> handler) {
        std::cout << "RegisterPacketHandler type=" << typeid(handler) << std::endl;
    }
*/

    template<typename T>
    struct PacketTypeFromIPacketHandler;

	template<typename TPacket>
    struct PacketTypeFromIPacketHandler<IPacketHandler<TPacket>> {
        using type = TPacket;
    };

    template<typename T>
    struct InheritsFromIPacketHandler {
    private:
        // Helper function to check if T inherits from IPacketHandler
        template<typename U>
        //static std::true_type test(IPacketHandler<typename U::packet_type>*);
		static std::true_type test(IPacketHandler<typename PacketTypeFromIPacketHandler<U>::type>*);
        
        // Overload for types that do not inherit from IPacketHandler
        template<typename>
        static std::false_type test(...);
        
    public:
        // Static constant indicating whether T inherits from IPacketHandler
        static constexpr bool value = decltype(test<T>(nullptr))::value;
    };

	//template<TPacket , typename THandler, typename = std::enable_if_t<std::is_base_of_v<Network::IPacketHandler<TPacket>, THandler>>>
	//template<typename TPacket , typename THandler>
	//template<ConceptPacket TPacket = int, typename THandler>
	//std::enable_if_t<std::is_base_of_v<IPacketHandler<TPacket>, THandler>>
	template<typename TPlugin, typename = std::enable_if_t<InheritsFromIPacketHandler<TPlugin>::value>>
    void RegisterPacketHandler(std::shared_ptr<TPlugin> handler) {
        std::cout << "RegisterPacketHandler type=" << std::endl; // << typeid(handler) << std::endl;
		//Network::IPacketHandler<TPacket>& test = *std::static_pointer_cast<IPacketHandler<TPacket>>(handler);
    }
}

//struct Plugin2 : public Network::IPacketHandler<int>, public Network::IPacketHandler<float> {
struct Plugin2 : public Network::IPacketHandler<int> {
	using packet_type = int;
};

int main() {
    std::shared_ptr<Plugin2> handler2 = std::make_shared<Plugin2>();
    Network::RegisterPacketHandler(handler2);

    return 0;
}

