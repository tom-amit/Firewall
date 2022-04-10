#include "AppCore/App.h"
#include "AppCore/Window.h"
#include "AppCore/Overlay.h"

using namespace ultralight;

const char *htmlString();

///
/// Welcome to Sample 2!
///
/// In this sample we'll introduce the AppCore API and use it to build a
/// simple application that creates a window and displays a string of HTML.
///
/// __What is AppCore?__
///
/// AppCore is an optional, high-performance, cross-platform application
/// framework built on top of the cmake-ul renderer.
///
/// It can be used to create standalone, GPU-accelerated HTML applications
/// that paint directly to the native window's backbuffer using the best
/// technology available on each platform (D3D, Metal, OpenGL, etc.).
///
/// We will create the simplest possible AppCore application in this sample.
///
int main() {
    ///
    /// Create our main App instance.
    ///
    /// The App class is responsible for the lifetime of the application
    /// and is required to create any windows.
    ///
    Ref<App> app = App::Create();

    ///
    /// Create our Window.
    ///
    /// This command creates a native platform window and shows it immediately.
    ///
    /// The window's size (400 by 400) is in virtual device coordinates, the
    /// actual size in pixels is automatically determined by the monitor's DPI.
    ///
    Ref<Window> window = Window::Create(app->main_monitor(), 900, 600, false,
                                        kWindowFlags_Titled);

    ///
    /// Set the title of our window.
    ///
    window->SetTitle("cmake-ul Sample 2 - Basic App");

    ///
    /// Tell our app to use 'window' as our main window.
    ///
    /// This call is required before creating any overlays or calling App::Run
    ///
    /// **Note**:
    ///   As of v1.1, AppCore only supports one window per application which is
    ///   why this call is required. This API method will be deprecated once
    ///   multi-monitor and multi-window support is added in v1.2.
    ///
    app->set_window(window);

    ///
    /// Create a web-content overlay that spans the entire window.
    ///
    /// You can create multiple overlays per window, each overlay has its own
    /// View which can be used to load and display web-content.
    ///
    /// AppCore automatically manages focus, keyboard/mouse input, and GPU
    /// painting for each active overlay. Destroying the overlay will remove
    /// it from the window.
    ///
    Ref<Overlay> overlay = Overlay::Create(window, window->width(),
                                           window->height(), 0, 0);

    ///
    /// Load a string of HTML into our overlay's View
    ///
    overlay->view()->LoadHTML(htmlString());

    ///
    /// Run our main loop.
    ///
    app->Run();

    return 0;
}

const char *htmlString() {
    return R"(
)";
}
