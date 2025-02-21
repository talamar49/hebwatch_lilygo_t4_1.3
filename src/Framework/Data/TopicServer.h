#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

class TopicServer {
public:
    // Publishes a message
    template<typename T>
    inline void Publish(const std::string& topic, const T& message);

    inline void Publish(const std::string& topic);

    // Subscribes to a topic
    template<typename T>
    inline void Subscribe(const std::string& topic, std::function<void(const T&)> callback);

    inline void Subscribe(const std::string& topic, std::function<void()> callback);

    // Collects all topic names
    std::vector<std::string> GetTopics() const;

    void PrintTopics() const ;


private:
    // Function to handle messages of any type
    using Callback = std::function<void(void*)>;

    // Map to hold subscriptions
    std::unordered_map<std::string, std::vector<Callback>> subscriptions;

    // Helper function to cast messages
    template<typename T>
    static void callCallback(const Callback& callback, const T& message);
};

// Publishes a message
template<typename T>
inline void TopicServer::Publish(const std::string& topic, const T& message) {
    auto it = subscriptions.find(topic);
    if (it != subscriptions.end()) {
        for (auto& subscriber : it->second) {
            callCallback(subscriber, message);
        }
    }    
}

// Publishes a message (for void)
inline void TopicServer::Publish(const std::string& topic) {
    auto it = subscriptions.find(topic);
    if (it != subscriptions.end()) {
        for (auto& subscriber : it->second) {
            subscriber(nullptr); // Pass nullptr for void callbacks
        }
    }
}

// Subscribes to a topic
template<typename T>
inline void TopicServer::Subscribe(const std::string& topic, std::function<void(const T&)> callback) {
    subscriptions[topic].push_back([callback](void* msgPtr) {
        callback(*static_cast<T*>(msgPtr));
    });    
}

// Subscribes to a topic with void
inline void TopicServer::Subscribe(const std::string& topic, std::function<void()> callback) {
    subscriptions[topic].push_back([callback](void*) {
        callback(); // Call the void callback
    });
}

// Collects all topic names
inline std::vector<std::string> TopicServer::GetTopics() const {
    std::vector<std::string> topicNames;
    for (const auto& pair : subscriptions) {
        topicNames.push_back(pair.first);
    }
    return topicNames;    
}

inline void TopicServer::PrintTopics() const {
    std::vector<std::string> topics = GetTopics();
    std::cout << "List of Topics:" << std::endl;
    for (const auto& topic : topics) {
        std::cout << "- " << topic << std::endl;
    }
}

// Helper function to call the callback with the proper type
template<typename T>
inline void TopicServer::callCallback(const Callback& callback, const T& message) {
    callback(const_cast<T*>(&message));
}