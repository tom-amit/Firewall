#include "AppCore/App.h"
#include "AppCore/Window.h"
#include "AppCore/Overlay.h"
#include "AppCore/JSHelpers.h"
#include "include/Controller.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace ultralight;

///
///  Welcome to Sample 4!
///
///  In this sample we'll show how to integrate C++ code with JavaScript.
///
///  We will introduce the DOMReady event and use it to bind a C++ callback to
///  a JavaScript function on our page. Later, when that callback is triggered,
///  we will execute some JavaScript to update a hidden message on the page.
///
///  __JavaScriptCore and AppCore__
///
///  Ultralight integrates the full JavaScriptCore engine (the same JS engine
///  in Safari on macOS and iOS). The SDK includes full C API bindings to
///  JavaScriptCore which gives you a great deal of flexibility but can be a
///  little daunting for first-time users.
///
///  To help simplify things, AppCore provides a simple C++ wrapper over
///  JavaScriptCore (@see JSHelpers.h). We'll be using this wrapper for the
///  sake of convenience in this sample.
///
class GUI : public LoadListener, ViewListener {
    RefPtr<Overlay> overlay_;
    Controller control;
public:
    explicit GUI(Ref<Window> win) {
        control = Controller();
        control.start({});

        ///
        /// Create an Overlay using the same dimensions as our Window.
        ///
        overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);

        ///
        /// Register our GUI instance as a load listener so we can handle the
        /// View's OnDOMReady event below.
        ///
        overlay_->view()->set_load_listener(this);
        overlay_->view()->set_view_listener(this);
        std::cout << ("For 121sdsssadsdass") << std::endl;
        ///
        /// Load a string of HTML (we're using a C++11 Raw String Literal)
        ///
        overlay_->view()->LoadURL("file:///table.html");
    }

	~GUI() override {
		control.stop({});
	}

    //TODO add support for RuleRemoval (note that we want to support multiple rule removal!)
    ///
    /// Our native JavaScript callback. This function will be called from
    /// JavaScript by calling GetMessage(). We bind the callback within
    /// the DOMReady callback defined below.
    ///
    JSValue AddRule(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        bool ret;
        if (args.empty()){
            ret = control.add_rule({});
        }
        else{
            ret = control.add_rule(
                    {args_str[0], args_str[1], args_str[2], args_str[3], args_str[4], args_str[5], args_str[6], args_str[7],
                     args_str[8]});
        }
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }

    JSValue EditRule(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        bool ret = control.edit_rule(
                {args_str[0], args_str[1], args_str[2], args_str[3], args_str[4], args_str[5], args_str[6], args_str[7],
                 args_str[8], args_str[9]});
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }
    JSValue SaveRules(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        args_str[0] += ".json";
        std::ofstream outfile;
        outfile.open(args_str[0], std::ios_base::app & std::ofstream::trunc);
        outfile << args_str[1] << std::endl; //
        outfile.close();

        return JSValueMakeBoolean(thisObject.context(), true);
    }
    JSValue LoadRules(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }

        std::ifstream infile(args_str[0]);
        string line;
        string content;
        while (std::getline(infile, line)) {
            content += line + "\n";
        }
        control.reset_firewall();
        return JSValueMakeString(thisObject.context(), JSStringCreateWithUTF8CString(content.c_str()));
    }
    JSValue RetrieveHitCounts(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        std::vector<uint64_t > ret = control.get_hit_counts();
        //return ret as a JS array
        JSValue array = JSObjectMakeArray(thisObject.context(), 0, nullptr, nullptr);
        for (int i = 0; i < ret.size(); ++i) {
            JSValue val = JSValueMakeNumber(thisObject.context(), ret[i]);
            JSObjectSetPropertyAtIndex(thisObject.context(), array, i, val, nullptr);
        }
        return array;
    }
    JSValue SwapRuleTo(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        bool ret = control.swap_rule_to(
                {args_str[0], args_str[1]});
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }

    JSValue RemoveRule(const JSObject &thisObject, const JSArgs &args) {
        ///
        /// Return our message to JavaScript as a JSValue.
        ///

        std::vector<string> args_str;

        for (int i = 0; i < args.size(); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        bool ret = control.remove_rule(
                {args_str[0]});
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }
    ///
    /// Inherited from LoadListener, called when the page has finished parsing
    /// the document.
    ///
    /// We perform all our JavaScript initialization here.
    ///
    void OnDOMReady(ultralight::View *caller,
                    uint64_t frame_id,
                    bool is_main_frame,
                    const String &url) override {
        ///
        /// Set our View's JSContext as the one to use in subsequent JSHelper calls
        ///
        Ref<JSContext> context = caller->LockJSContext();
        SetJSContext(context.get());

        ///
        /// Get the global object (this would be the "window" object in JS)
        ///
        JSObject global = JSGlobalObject();

        ///
        /// Bind GUI::GetMessage to the JavaScript function named "GetMessage".
        ///
        /// You can get/set properties of JSObjects by using the [] operator with
        /// the following types as potential property values:
        ///  - JSValue
        ///      Represents a JavaScript value, eg String, Object, Function, etc.
        ///  - JSCallback
        ///      Typedef of std::function<void(const JSObject&, const JSArgs&)>)
        ///  - JSCallbackWithRetval
        ///      Typedef of std::function<JSValue(const JSObject&, const JSArgs&)>)
        ///
        /// We use the BindJSCallbackWithRetval macro to bind our C++ class member
        /// function to our JavaScript callback.
        ///
        global["AddRule"] = BindJSCallbackWithRetval(&GUI::AddRule);
        global["EditRule"] = BindJSCallbackWithRetval(&GUI::EditRule);
        global["RemoveRule"] = BindJSCallbackWithRetval(&GUI::RemoveRule);
        global["SwapRuleTo"] = BindJSCallbackWithRetval(&GUI::SwapRuleTo);
        global["SaveRules"] = BindJSCallbackWithRetval(&GUI::SaveRules);
        global["LoadRules"] = BindJSCallbackWithRetval(&GUI::LoadRules);
        global["RetrieveHitCounts"] = BindJSCallbackWithRetval(&GUI::RetrieveHitCounts);
    }

    static inline std::string ToUTF8(const String &str) {
        String8 utf8 = str.utf8();
        return {utf8.data(), utf8.length()};
    }

    static inline const char *Stringify(MessageSource source) {
        switch (source) {
            case kMessageSource_XML:
                return "XML";
            case kMessageSource_JS:
                return "JS";
            case kMessageSource_Network:
                return "Network";
            case kMessageSource_ConsoleAPI:
                return "ConsoleAPI";
            case kMessageSource_Storage:
                return "Storage";
            case kMessageSource_AppCache:
                return "AppCache";
            case kMessageSource_Rendering:
                return "Rendering";
            case kMessageSource_CSS:
                return "CSS";
            case kMessageSource_Security:
                return "Security";
            case kMessageSource_ContentBlocker:
                return "ContentBlocker";
            case kMessageSource_Other:
                return "Other";
            default:
                return "";
        }
    }

    static inline const char *Stringify(MessageLevel level) {
        switch (level) {
            case kMessageLevel_Log:
                return "Log";
            case kMessageLevel_Warning:
                return "Warning";
            case kMessageLevel_Error:
                return "Error";
            case kMessageLevel_Debug:
                return "Debug";
            case kMessageLevel_Info:
                return "Info";
            default:
                return "";
        }
    }

    void OnAddConsoleMessage(View *caller,
                             MessageSource source,
                             MessageLevel level,
                             const String &message,
                             uint32_t line_number,
                             uint32_t column_number,
                             const String &source_id) override {

        std::cout << "[Console]: [" << Stringify(source) << "] ["
                  << Stringify(level) << "] " << ToUTF8(message);

        if (source == kMessageSource_JS) {
            std::cout << " (" << ToUTF8(source_id) << " @ line " << line_number
                      << ", col " << column_number << ")";
        }

        std::cout << std::endl;

    }
};

int main() {
    ///
    /// Create our main App instance.
    ///
    NICS::GetData("ens33", "ens36");
    auto app = App::Create();

    ///
    /// Create our Window using default window flags.
    ///
    auto window = Window::Create(app->main_monitor(), 1400, 800, false, kWindowFlags_Titled);

    ///
    /// Set our window title.
    ///
    window->SetTitle("Firewall - Rule Table");

    ///
    /// Bind our App's main window.
    ///
    /// @note This MUST be done before creating any overlays or calling App::Run
    ///
    app->set_window(window);
    ///
    /// Create our GUI instance (creates overlays and handles all logic).
    ///
    GUI my_app(window);
    ///
    /// Run our main loop.
    ///
    app->Run();

    return 0;
}
//
// Inherited from ViewListener::OnAddConsoleMessage
//
// Make sure that you bind 'GUI' to 'View::set_view_listener'
// to receive this event.
//