#include "AppCore/App.h"
#include "AppCore/Window.h"
#include "AppCore/Overlay.h"
#include "AppCore/JSHelpers.h"
#include "include/Controller.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <net/if.h>
#include <cmath>

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
        //control.start({});

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
        ///
        /// Load a string of HTML (we're using a C++11 Raw String Literal)
        ///
        overlay_->view()->LoadURL("file:///table.html");
    }

	/**
	 Destructor for GUI
	 */
	~GUI() override {

		control.stop({});
	}

    ///
    /// Our native JavaScript callback. This function will be called from
    /// JavaScript by calling GetMessage(). We bind the callback within
    /// the DOMReady callback defined below.
    ///


	/**
	 * When a rule is added to the table, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */

    JSValue AddRule(const JSObject &thisObject, const JSArgs &args) {
        std::vector<string> args_str;

        for (int i = 0; i < std::min((int)args.size(), 8); ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        bool ret;
        if (args.empty()){
            ret = control.add_rule({});
        }//
        else{
            ret = control.add_rule(
                    {args_str[0], args_str[1], args_str[2], args_str[3], args_str[4], args_str[5], args_str[6], args_str[7]});//
        }
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }

	/**
	 * When a rule is edited in the table, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */

	JSValue EditRule(const JSObject &thisObject, const JSArgs &args) {
        std::vector<string> args_str;

        for (int i = 0; i < 9; ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
        bool ret = control.edit_rule(
                {args_str[0], args_str[1], args_str[2], args_str[3], args_str[4], args_str[5], args_str[6], args_str[7],
                 args_str[8]});
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }
	/**
	 * When a the rule table is saved, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */

	const string dir = "../../rules";

	JSValue SaveRules(const JSObject &thisObject, const JSArgs &args) {
        std::vector<string> args_str;

        for (int i = 0; i < 2; ++i) {
            JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
            ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                         (size_t) JSStringGetLength(s));
            std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
            args_str.push_back(str);
        }
		if (args_str[0].size() < 5 || args_str[0].substr(args_str[0].size() - 5) != ".json")
			args_str[0] += ".json";
		args_str[0] = dir + "/" + args_str[0];
        std::ofstream outfile;
        outfile.open(args_str[0],  std::ios::out | std::ios::in | std::ios::trunc);
        outfile << args_str[1] << std::endl;
        outfile.close();
		system(("chmod 777 " + args_str[0]).c_str());
        return JSValueMakeBoolean(thisObject.context(), true);
    }

	/**
	 * When a the rule table is loaded, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */
    JSValue LoadRules(const JSObject &thisObject, const JSArgs &args) {
        string args_str;
		//
        JSString s = JSValueToStringCopy(thisObject.context(), args[0], nullptr);
        ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                     (size_t) JSStringGetLength(s));
        args_str  = std::string((char *) ustr.utf8().data(), ustr.utf8().length());

		if (args_str.size() < 5 || args_str.substr(args_str.size() - 5) != ".json")
			args_str += ".json";
		args_str = dir + "/" + args_str;


        std::ifstream infile(args_str);
	    if (!infile.good()) {
		    return JSValueMakeString(thisObject.context(), JSStringCreateWithUTF8CString(""));
	    }
	    string line;
        string content;
        while (std::getline(infile, line)) {
            content += line + "\n";
        }
		infile.close();
        control.reset_firewall();
        return JSValueMakeString(thisObject.context(), JSStringCreateWithUTF8CString(content.c_str()));
    }
	/**
	 * Retrieves the hit count for each rule.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */
    JSValue RetrieveHitCounts(const JSObject &thisObject, const JSArgs &args) {
        std::vector<uint64_t > ret = control.get_hit_counts();
        JSValue array = JSObjectMakeArray(thisObject.context(), 0, nullptr, nullptr);
        for (int i = 0; i < ret.size(); ++i) {
            JSValue val = JSValueMakeNumber(thisObject.context(), ret[i]);
            JSObjectSetPropertyAtIndex(thisObject.context(), array, i, val, nullptr);
        }
        return array;
    }
    JSValue SwapRuleTo(const JSObject &thisObject, const JSArgs &args) {
        std::vector<string> args_str;

        for (int i = 0; i < 2; ++i) {
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

	/**
	 * When a rule/s is/are removed, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */
    JSValue RemoveRule(const JSObject &thisObject, const JSArgs &args) {
        string args_str;

        JSString s = JSValueToStringCopy(thisObject.context(), args[0], nullptr);
        ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
                                                     (size_t) JSStringGetLength(s));
        args_str  = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
        bool ret = control.remove_rule(
                {args_str});
        if (ret) {
            return JSValueMakeBoolean(thisObject.context(), true);
        } else {
            return JSValueMakeBoolean(thisObject.context(), false);
        }
    }
	/**
	 Extracts all the network interfaces from the system.
	 */
	static std::vector<string>  extract_network_interface_names()
	{
		/* DEFINITION
		 *  #include <net/if.h>
		 *  struct if_nameindex * if_nameindex() :  function that returns a pointer.
		 *  The if_nameindex()  function returns an array of if_nameindex structs. There is one struct for each network interface
		 *  The if_nameindex struct contains at least the following members:
		 *      unsigned int if_index : which is the index of each network interface {1,2,..n}
		 *      char * if_name : which is the name of the interface=the C style string= an array of characters terminated in the NULL character \0
		 *
		 *  The end of the array of struct is indicated by an entry with if_index=0 && if_name=NULL
		 *  The if_nameindex() function returns an array of structs if successful, or NULL if some error occurred or not enough memory is available
		 */
		puts("Extracting network interface names...");
		puts("Listing all available network interface names on this machine...");
		puts("********************************");
		std::vector<string> ret = {};
		int counter_network_interfaces=0;

		struct if_nameindex * interface_indexes;
		struct if_nameindex * interface;

		interface_indexes=if_nameindex();

		if(interface_indexes==nullptr) //Some error occurred during the execution of if_nameindex() function or there is no enough memory available
		{
			perror("If_nameindex");
			printf("Ooops...error while extracting the network interface names.\n");
			exit(EXIT_FAILURE);
		}

		//Loop through the elements of the array of if_nameindex structs

		for(interface=interface_indexes;interface->if_index!=0 && interface->if_name!=nullptr;interface++)
		{
			counter_network_interfaces++;
			printf("There exists a network interface called \"%s\" with index %d. \n",interface->if_name, interface->if_index);
			ret.emplace_back(interface->if_name);
		}

		puts("*******************************");
		printf("In total, there is a number of %d network interfaces on this machine.\n",counter_network_interfaces);
		return ret;


	}
	/**
	 * When requesting the list of NICS, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */
	JSValue RequestNICS(const JSObject &thisObject, const JSArgs &args) {
		std::vector<string> ret = extract_network_interface_names();
		JSValue array = JSObjectMakeArray(thisObject.context(), 0, nullptr, nullptr);
		for (int i = 0; i < ret.size(); ++i) {
			JSValue val = JSValueMakeString(thisObject.context(), JSStringCreateWithUTF8CString(ret[i].c_str()));
			JSObjectSetPropertyAtIndex(thisObject.context(), array, i, val, nullptr);
		}
		return array;
	}

	/**
	 * When registering the NICS selected, this function is called.
	 * @param thisObject, a reference to the object that called the function.
	 * @param args, the arguments passed to the function.
	 * @return a JSValue representing the result of the function.
	 */
	JSValue RegisterNICS(const JSObject &thisObject, const JSArgs &args) {
		std::vector<string> args_str;

		for (int i = 0; i < 2; ++i) {
			JSString s = JSValueToStringCopy(thisObject.context(), args[i], nullptr);
			ultralight::String ustr = ultralight::String((Char16 *) JSStringGetCharactersPtr(s),
				(size_t) JSStringGetLength(s));
			std::string str = std::string((char *) ustr.utf8().data(), ustr.utf8().length());
			args_str.push_back(str);
		}
		bool ret = control.changeNICS(
				{args_str[0], args_str[1]});
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
	    //
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
		global["RequestNICS"] = BindJSCallbackWithRetval(&GUI::RequestNICS);
		global["RegisterNICS"] = BindJSCallbackWithRetval(&GUI::RegisterNICS);
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
    //NICS::GetData("ens33", "ens36");
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