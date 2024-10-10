#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <functional>

// Global variable to store main thread ID
std::thread::id main_thread_id;

// Client's callback function
void clientCallback() {
    std::thread::id callback_thread_id = std::this_thread::get_id();
    std::cout << "Callback is running in thread ID: " << callback_thread_id << std::endl;
    
    if (callback_thread_id == main_thread_id) {
        std::cout << "Callback is running in the MAIN thread." << std::endl;
    } else {
        std::cout << "Callback is running in a DIFFERENT thread." << std::endl;
    }
}

int main() {
    // Capture the main thread ID
    main_thread_id = std::this_thread::get_id();
    std::cout << "Main thread ID: " << main_thread_id << std::endl;

    // Simulate library behavior with its own thread
    std::thread library_thread([](){
        std::cout << "Library thread ID: " << std::this_thread::get_id() << std::endl;
        // Directly invoke the callback
        clientCallback();
    });

    library_thread.join();

    return 0;
}
