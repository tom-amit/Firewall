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
<html>
  <head>
    <style type="text/css">
.og-contianer {
  max-width: 1200px;
  margin: auto;
  box-shadow: 0 0.125rem 0.25rem rgba(0, 0, 0, 0.075) !important;
  background: #1c1f21 !important;
  padding: 30px;
  border-radius: 10px;
}
.heading-line {
  position: relative;
  display: block;
  padding: 0px 0px 10px 0px;
  font-weight: 500;
  text-decoration: none;
  transition: 0.35s ease;
  border-bottom: 3px solid transparent;
  font-size: 48px;
  color: #fff;
}
.heading-line:after {
  position: absolute;
  display: inline-block;
  background: #efa617;
  content: " ";
  height: 0;
  transition: 0.35s ease;
  bottom: 0;
  width: 100px;
  height: 3px;
  margin: 0 auto;
  left: 0;
  transform: none;
}
.og-row {
  display: flex;
  flex-wrap: wrap;
  padding: 15px 0px;
}
.og-row select {
  padding: 10px;
  margin-right: 15px;
  margin-bottom: 10px;
  border-radius: 5px;
  border: 1px solid #222629;
  background-color: #222629;
  color: #fff;
  box-shadow: 0 0.125rem 0.25rem rgba(0, 0, 0, 0.075) !important;
}
.text-center {
  text-align: center;
}
.og-li {
  border-bottom: 1px solid #3e4042;
  color: #fff;
  font-size: 18px;
}
og-li-col {
  padding: 0px 10px;
}
.og-li-col-1 {
  width: 5%;
}
.og-li-col-2 {
  width: 15%;
}
.og-li-col-3 {
  width: 13%;
}
.og-li-col-4 {
  width: 13%;
}
.og-li-col-5 {
  width: 13%;
}
.og-li-col-6 {
  width: 15%;
}
.og-li-col-7 {
  width: 13%;
}
.og-li-col-8 {
  width: 13%;
}
.og-li-head {
  font-size: 20px;
  font-weight: 700;
}
.hide {
  display: none;
}
.no-match {
  padding: 20px;
  font-size: 24px;
}

    </style>
    <script>
document.getElementById("og-filters").onchange = function () {
  var dataRows = document.getElementsByClassName("data-row");
  add_class_hide(dataRows);
  var experience = document.getElementById("experience").value;
  var education = document.getElementById("education").value;
  var availability = document.getElementById("availability").value;
  var relocation = document.getElementById("relocation").value;
  var location = document.getElementById("location").value;
  var filterValues = new Array(
    experience,
    education,
    availability,
    relocation,
    location
  );
  var filteredBy = filterValues.filter(Boolean);
  var flag = 0;
  var flag_no = 0;

  for (i = 0; i < dataRows.length; i++) {
    for (j = 0; j < filteredBy.length; j++) {
      if (dataRows[i].classList.contains(filteredBy[j])) {
        flag++;
      } else {
        flag--;
      }
    }
    if (flag == filteredBy.length) {
      dataRows[i].classList.remove("hide");
    }
    flag = 0;
  }
  for (k = 0; k < dataRows.length; k++) {
    if (dataRows[k].classList.contains("hide")) {
      flag_no++;
    } else {
      flag_no--;
    }
  }
  if (dataRows.length == flag_no) {
    document.getElementById("no-match").classList.remove("hide");
  }
};

function remove_class_hide(selectedRows) {
  for (a = 0; a < selectedRows.length; a++) {
    selectedRows[a].classList.remove("hide");
  }
}
function add_class_hide(selectedRows) {
  for (b = 0; b < selectedRows.length; b++) {
    selectedRows[b].classList.add("hide");
  }
}

  </script>
  </head>
  <body>
    <div class="og-contianer">
  <h1 class="heading-line">Odin Grads</h1>
  <div class="og-row" id="og-filters">
    <select class="" id="education" placeholder="Education">
      <option value="" selected="">Education</option>
      <option value="Engineering">Engineering</option>
      <option value="Non-Engineering">Non-Engineering</option>
    </select>
    <select class="" id="experience" placeholder="Experience">
      <option value="" selected="">Experience</option>
      <option value="Experienced">Experienced</option>
      <option value="Fresher">Fresher</option>
    </select>
    <select class="" id="availability" placeholder="Availability">
      <option value="" selected="">Availability</option>
      <option value="ready_to_hire">Ready to hire</option>
      <option value="graduating_in_shortly">Graduating in shortly</option>
    </select>
    <select class="" id="relocation" placeholder="Relocation">
      <option value="" selected="">Relocation</option>
      <option value="Yes">Yes</option>
      <option value="No">No</option>
    </select>
    <select name="state" id="location" class="form-control">
      <option value="">Location</option>
      <option value="Andhra Pradesh">Andhra Pradesh</option>
      <option value="Andaman and Nicobar Islands">Andaman and Nicobar Islands</option>
      <option value="Arunachal Pradesh">Arunachal Pradesh</option>
      <option value="Assam">Assam</option>
      <option value="Bihar">Bihar</option>
      <option value="Chandigarh">Chandigarh</option>
      <option value="Chhattisgarh">Chhattisgarh</option>
      <option value="Dadar and Nagar Haveli">Dadar and Nagar Haveli</option>
      <option value="Daman and Diu">Daman and Diu</option>
      <option value="Delhi">Delhi</option>
      <option value="Lakshadweep">Lakshadweep</option>
      <option value="Puducherry">Puducherry</option>
      <option value="Goa">Goa</option>
      <option value="Gujarat">Gujarat</option>
      <option value="Haryana">Haryana</option>
      <option value="Himachal Pradesh">Himachal Pradesh</option>
      <option value="Jammu and Kashmir">Jammu and Kashmir</option>
      <option value="Jharkhand">Jharkhand</option>
      <option value="Karnataka">Karnataka</option>
      <option value="Kerala">Kerala</option>
      <option value="Madhya Pradesh">Madhya Pradesh</option>
      <option value="Maharashtra">Maharashtra</option>
      <option value="Manipur">Manipur</option>
      <option value="Meghalaya">Meghalaya</option>
      <option value="Mizoram">Mizoram</option>
      <option value="Nagaland">Nagaland</option>
      <option value="Odisha">Odisha</option>
      <option value="Punjab">Punjab</option>
      <option value="Rajasthan">Rajasthan</option>
      <option value="Sikkim">Sikkim</option>
      <option value="Tamil Nadu">Tamil Nadu</option>
      <option value="Telangana">Telangana</option>
      <option value="Tripura">Tripura</option>
      <option value="Uttar Pradesh">Uttar Pradesh</option>
      <option value="Uttarakhand">Uttarakhand</option>
      <option value="West Bengal">West Bengal</option>
    </select>
  </div>
  <div class="og-row og-li og-li-head">
    <div class="og-li-col og-li-col-1 text-center">#</div>
    <div class="og-li-col og-li-col-2">Name</div>
    <div class="og-li-col og-li-col-3 text-center">Education</div>
    <div class="og-li-col og-li-col-4 text-center">Experience</div>
    <div class="og-li-col og-li-col-5 text-center">Bootcamp CGPA</div>
    <div class="og-li-col og-li-col-6 text-center">Availability</div>
    <div class="og-li-col og-li-col-7 text-center">Location</div>
    <div class="og-li-col og-li-col-8 text-center">Relocation</div>
  </div>
  <div class="data-row og-row og-li Experienced Engineering 7.3 ready_to_hire Andhra Pradesh Yes">
    <div class="og-li-col og-li-col-1 text-center">1</div>
    <div class="og-li-col og-li-col-2">Rohit Palem</div>
    <div class="og-li-col og-li-col-3 text-center">Engineering</div>
    <div class="og-li-col og-li-col-4 text-center">Experienced</div>
    <div class="og-li-col og-li-col-5 text-center">7.3</div>
    <div class="og-li-col og-li-col-6 text-center">Ready to hire</div>
    <div class="og-li-col og-li-col-7 text-center">Andhra Pradesh</div>
    <div class="og-li-col og-li-col-8 text-center">Yes</div>
  </div>
  <div class="data-row og-row og-li Fresher Engineering 7 graduating_in_shortly Andhra Pradesh Yes">
    <div class="og-li-col og-li-col-1 text-center">2</div>
    <div class="og-li-col og-li-col-2">Mayank</div>
    <div class="og-li-col og-li-col-3 text-center">Engineering</div>
    <div class="og-li-col og-li-col-4 text-center">Fresher</div>
    <div class="og-li-col og-li-col-5 text-center">7</div>
    <div class="og-li-col og-li-col-6 text-center">Graduating in shortly</div>
    <div class="og-li-col og-li-col-7 text-center">Andhra Pradesh</div>
    <div class="og-li-col og-li-col-8 text-center">Yes</div>
  </div>
  <div class="data-row og-row og-li Experienced Non-Engineering 8 ready_to_hire Andhra Pradesh No">
    <div class="og-li-col og-li-col-1 text-center">3</div>
    <div class="og-li-col og-li-col-2">Faviana</div>
    <div class="og-li-col og-li-col-3 text-center">Non-Engineering</div>
    <div class="og-li-col og-li-col-4 text-center">Experienced</div>
    <div class="og-li-col og-li-col-5 text-center">8</div>
    <div class="og-li-col og-li-col-6 text-center">Ready to hire</div>
    <div class="og-li-col og-li-col-7 text-center">Andhra Pradesh</div>
    <div class="og-li-col og-li-col-8 text-center">No</div>
  </div>
  <div class="data-row og-row og-li Fresher Engineering 6 graduating_in_shortly Andhra Pradesh No">
    <div class="og-li-col og-li-col-1 text-center">4</div>
    <div class="og-li-col og-li-col-2">Harsha</div>
    <div class="og-li-col og-li-col-3 text-center">Engineering</div>
    <div class="og-li-col og-li-col-4 text-center">Fresher</div>
    <div class="og-li-col og-li-col-5 text-center">6</div>
    <div class="og-li-col og-li-col-6 text-center">Graduating in shortly</div>
    <div class="og-li-col og-li-col-7 text-center">Andhra Pradesh</div>
    <div class="og-li-col og-li-col-8 text-center">No</div>
  </div>
  <div class="data-row og-row og-li Experienced Non-Engineering 7 ready_to_hire Andhra Pradesh Yes">
    <div class="og-li-col og-li-col-1 text-center">5</div>
    <div class="og-li-col og-li-col-2">Deepa</div>
    <div class="og-li-col og-li-col-3 text-center">Non-Engineering</div>
    <div class="og-li-col og-li-col-4 text-center">Experienced</div>
    <div class="og-li-col og-li-col-5 text-center">7</div>
    <div class="og-li-col og-li-col-6 text-center">Ready to hire</div>
    <div class="og-li-col og-li-col-7 text-center">Andhra Pradesh</div>
    <div class="og-li-col og-li-col-8 text-center">Yes</div>
  </div>
  <div class="data-row og-row og-li Experienced Non-Engineering 7 ready_to_hire Andhra Pradesh Yes">
    <div class="og-li-col og-li-col-1 text-center">6</div>
    <div class="og-li-col og-li-col-2">Harsha</div>
    <div class="og-li-col og-li-col-3 text-center">Non-Engineering</div>
    <div class="og-li-col og-li-col-4 text-center">Experienced</div>
    <div class="og-li-col og-li-col-5 text-center">7</div>
    <div class="og-li-col og-li-col-6 text-center">Ready to hire</div>
    <div class="og-li-col og-li-col-7 text-center">Andhra Pradesh</div>
    <div class="og-li-col og-li-col-8 text-center">Yes</div>
  </div>
  <div id="no-match" class="no-match og-li  text-center hide">Sorry, No Student Matches your Criteria</div>
</div>
  </body>
</html>
)";
}
