#include "astra/astra.hpp"

#include "gloo/init.hpp"

#include "sdl3_raii/gl_attr.hpp"
#include "sdl3_raii/gl_context_flags.hpp"
#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

#define ASTRA_TAG_MEMBER(name) constexpr static std::uint32_t ASTRA_MESSENGER_TAG{astra::murmur_x86_32(#name, 0)};

struct MsgA {
    ASTRA_TAG_MEMBER(MsgA);
    int a;
    int b;
};

struct MsgB {
    ASTRA_TAG_MEMBER(MsgB);
    float a;
};

template<typename T>
const T *extract_payload(const std::span<const std::byte> &buffer) {
    return reinterpret_cast<const T *>(buffer.data());
}

template<typename T>
concept HasAstraTag = std::same_as<decltype(T::ASTRA_MESSENGER_TAG), const std::uint32_t>;

class Messenger {
    using Receiver = std::function<void(std::span<const std::byte>)>;

public:
    template<typename T>
        requires HasAstraTag<T>
    void subscribe(Receiver &&r);

    template<typename T, typename... Args>
        requires HasAstraTag<T>
    void publish(Args &&...args);

private:
    std::unordered_map<std::uint32_t, std::vector<Receiver>> receivers_;

    template<typename T, typename... Args>
    std::span<std::byte> make_payload_(Args &&...args);
};

template<typename T>
    requires HasAstraTag<T>
void Messenger::subscribe(Receiver &&r) {
    receivers_[T::ASTRA_MESSENGER_TAG].push_back(std::forward<Receiver>(r));
}

template<typename T, typename... Args>
    requires HasAstraTag<T>
void Messenger::publish(Args &&...args) {
    const auto payload = make_payload_<T>(std::forward<Args>(args)...);
    for (auto &r: receivers_[T::ASTRA_MESSENGER_TAG])
        r(payload);
    operator delete(payload.data(), payload.size());
}

template<typename T, typename... Args>
std::span<std::byte> Messenger::make_payload_(Args &&...args) {
    return std::span(reinterpret_cast<std::byte *>(new T{std::forward<Args>(args)...}), sizeof(T));
}

static int i = 0;

int main(int, char *[]) {
    Messenger messenger;
    messenger.subscribe<MsgA>([](const std::span<const std::byte> buffer) {
        const auto msg = extract_payload<MsgA>(buffer);
        i += msg->a + msg->b;
    });

    constexpr int N = 100'000'000;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i)
        messenger.publish<MsgA>(astra::rng::get<int>(5), astra::rng::get<int>(5));
    auto t2 = std::chrono::high_resolution_clock::now();
    fmt::println(
            "{:.03f} messages per second",
            (double)N / (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e9));

    fmt::println("{}", i);

    return 0;

    //     astra::log_platform();
    //
    //     if (!sdl3::init())
    //         return 1;
    //
    //     sdl3::GlAttr::set_context_version(4, 6);
    //     sdl3::GlAttr::set_context_profile(sdl3::GlProfile::Core);
    // #if !defined(NDEBUG)
    //     sdl3::GlAttr::set_context_flags().debug().set();
    // #endif
    //     auto window = sdl3::WindowBuilder("Indev", {1280, 720}).opengl().build();
    //
    //     gloo::init();
    //
    //     bool running = true;
    //     while (running) {
    //         SDL_Event event;
    //         while (SDL_PollEvent(&event)) {
    //             switch (event.type) {
    //             case SDL_EVENT_QUIT: running = false; break;
    //             default: break;
    //             }
    //         }
    //
    //         window->swap();
    //     }
    //
    //     sdl3::exit();
}
