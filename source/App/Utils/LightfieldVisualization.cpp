/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     LightfieldVisualization.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-03-18
 */

#include <iostream>
#include "Lib/Part2/Common/LightfieldFromFile.h"
#include "Lib/Part2/Common/ViewIOPolicyLimitedMemory.h"
#include "Lib/Part2/Common/ViewIOPolicyLimitlessMemory.h"
#include "Lib/Utils/BasicConfiguration/BasicConfiguration.h"

//X11:
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

class LightfieldVisualizationConfiguration : public BasicConfiguration {
 private:
  static constexpr std::size_t current_hierarchy_level = 0;

 protected:
  std::string input;
  uint32_t number_of_rows_t;
  uint32_t number_of_columns_s;
  LightfieldVisualizationConfiguration(int argc, char **argv, std::size_t level)
      : BasicConfiguration(argc, argv, level) {
  }


  virtual void add_options() override {
    BasicConfiguration::add_options();


    this->add_cli_json_option({"--input", "-i",
        "Input directory that contains at least three subdirectories with PGX "
        "files.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("input")) {
            return conf["input"].get<std::string>();
          }
          return std::nullopt;
        },
        [this]([[maybe_unused]] std::any v) {
          this->input = std::any_cast<std::string>(v);
        },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--number_of_rows", "-t",
        "Number of light-field view rows. Mandatory.",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("number_of_rows")) {
            return std::to_string(conf["number_of_rows"].get<uint32_t>());
          }
          return std::nullopt;
        },
        [this](std::string arg) { this->number_of_rows_t = std::stoi(arg); },
        this->current_hierarchy_level});

    this->add_cli_json_option({"--number_of_columns", "-s",
        "Number of light-field view columns",
        [this](const nlohmann::json &conf) -> std::optional<std::string> {
          if (conf.contains("number_of_columns")) {
            return std::to_string(conf["number_of_columns"].get<uint32_t>());
          }
          return std::nullopt;
        },
        [this](std::string arg) { this->number_of_columns_s = std::stoi(arg); },
        this->current_hierarchy_level});
  }

 public:
  LightfieldVisualizationConfiguration(int argc, char **argv)
      : LightfieldVisualizationConfiguration(argc, argv,
            LightfieldVisualizationConfiguration::current_hierarchy_level) {
    this->init(argc, argv);
  }


  virtual ~LightfieldVisualizationConfiguration() = default;


  const std::string &get_input_filename() const {
    return input;
  }


  auto get_t() const {
    return this->number_of_rows_t;
  }


  auto get_s() const {
    return this->number_of_columns_s;
  }
};


class X11WindowHandler {
 protected:
  Display *display;
  Window window;
  GC graphics_context;  //! GC stands for Graphic Contexts
  XEvent event;
  KeySym key;
  char text[255];
  XImage *x_image_ptr;
  unsigned int width, height;

 public:
  X11WindowHandler(
      const std::string &window_name, unsigned int width, unsigned int height)
      : width(width), height(height) {
    display = XOpenDisplay(
        nullptr);  //! if display_name == NULL, it opens the value of env. DISPLAY (POSIX)
    auto screen_id = DefaultScreen(display);  //! the default screen number
    auto black = BlackPixel(display, screen_id);
    //, white = WhitePixel(display, screen);
    window = XCreateSimpleWindow(display,
        DefaultRootWindow(display),  //<! parent window
        0, 0,  //<! x, y
        width, height,
        5,  //<! border width in pixels
        black,  //! border pixel value
        black  //background
    );
    XSetStandardProperties(display, window, window_name.c_str(),
        "Test",  //! icon name?
        None,  //! icon pixmap
        nullptr,  //! argv
        0,  //! argc
        nullptr  //XSizeHints
    );

    XSelectInput(display, window,
        ExposureMask | ButtonPressMask | KeyPressMask | PointerMotionMask);

    graphics_context = XCreateGC(display, window, 0, 0);
    XSetBackground(display, graphics_context, black);
    XSetForeground(display, graphics_context, black);
    XClearWindow(display, window);
    XMapRaised(display, window);

    x_image_ptr = XCreateImage(display, DefaultVisual(display, 0),
        24,  //! depth of the image
        ZPixmap,  //! image format
        0,  //! number of pixels to ignore
        nullptr,  //! the image data
        width, height, 32, 0);
  }


  /**
   * @brief      Destroys the object.
   */
  ~X11WindowHandler() {
    XFreeGC(display, graphics_context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
  }


  /**
   * @brief      Clears the window
   */
  void clear_window() {
    XClearWindow(display, window);
  }


  /**
   * @brief      Redraws the window by redrawing the image pointed by x_image_ptr
   */
  virtual void redraw() {
    XPutImage(display, window, DefaultGC(display, 0), x_image_ptr, 0,
        0,  //! source x, y
        0, 0,  //! dest x, y
        width, height);
  }


  /**
   * @brief      Loops window waiting for any event to occur
   */
  void loop() {
    while (1) {
      auto pending = XPending(display);
      XNextEvent(display, &event);
      auto last_event = event;

      if (event.type == MotionNotify) {
        while (event.type == MotionNotify && pending > 1) {
          last_event = event;
          XNextEvent(display, &event);
          pending = XPending(display);
        }
        treat_mouse_movement(event.xbutton);

        continue;
      }
      if (event.type == Expose && event.xexpose.count == 0) {
        redraw();
        continue;
      }
      if (event.type == KeyPress &&
          XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
        if (text[0] == 'q') {
          break;
        }
        printf("You pressed the %c key! Press 'q' to quit. \n", text[0]);
      }
    }
  }

  virtual void treat_mouse_movement(const XButtonEvent &xbutton) = 0;
};


template<typename T>
class LightfieldAtX11Window : public X11WindowHandler {
 protected:
  Lightfield<T> *lightfield;  //! \todo Make the Lightfield<T>& const
  std::unique_ptr<unsigned char[]>
      image32;  //! ptr that stores the image which is shown
  std::pair<std::size_t, std::size_t> past_t_s = std::make_pair(0, 0);
  std::pair<double, double> view_size_wrt_lightfield = std::make_pair(1.0, 1.0);

 public:
  LightfieldAtX11Window(
      const std::string &window_name, Lightfield<T> *lightfield)
      : X11WindowHandler(window_name, lightfield->get_views_width_u(),
            lightfield->get_views_height_v()),
        lightfield(lightfield),
        image32(std::make_unique<unsigned char[]>(
            lightfield->get_number_of_pixels_per_view_channel() * 4)) {
    auto &[view_relative_horizontal_size, view_relative_vertical_size] =
        view_size_wrt_lightfield;
    view_relative_horizontal_size = lightfield->get_views_width_u() /
                                    ((double) lightfield->get_width() - 1);
    view_relative_vertical_size = lightfield->get_views_height_v() /
                                  ((double) lightfield->get_height() - 1);
    auto data_ptr = image32.get();
    this->x_image_ptr->data = reinterpret_cast<char *>(data_ptr);
  }


  /**
   * @brief      Redraws view into window
   */
  virtual void redraw() override {
    load_view_into_image(past_t_s);
    X11WindowHandler::redraw();
  }


  /**
   * @brief      Handler for the mouse movement
   *
   * @param[in]  xbutton  The xbutton
   */
  virtual void treat_mouse_movement(const XButtonEvent &xbutton) override {
    auto x = xbutton.x;
    auto y = xbutton.y;
    const auto &[view_relative_horizontal_size, view_relative_vertical_size] =
        view_size_wrt_lightfield;

    const auto positional_view_x = x / view_relative_horizontal_size;
    const auto positional_view_y = y / view_relative_vertical_size;

    const auto t = static_cast<std::size_t>(std::round(positional_view_y));
    const auto s = static_cast<std::size_t>(std::round(positional_view_x));

    auto &[past_t, past_s] = past_t_s;

    if ((past_t != t) || (past_s != s)) {
      past_t = t;
      past_s = s;
      redraw();
    }
  }


  /**
   * @brief      Convert from a uint16_t to uint8_t by performing a shift
   *             according to the original depth
   *
   * @param[in]  source  The source
   *
   * @tparam     depth   Original image depth (bpp)
   *
   * @return     The shifted source represented in the interval [0..255]
   */
  template<int depth = 10>
  uint8_t convert_from_16_bit_to_8_bit_using_depth(uint16_t source) {
    constexpr auto shift = depth - 8;
    return (source) >> shift;
  }


  /**
   * @brief      Loads a view into a x11 image.
   *
   * @param[in]  view_coordinate  The view coordinate
   *
   * @tparam     depth            Original view image depth (bpp)
   */
  template<int depth = 10>
  void load_view_into_image(
      const std::pair<std::size_t, std::size_t> &view_coordinate) {
    using namespace ImageColorSpaceConversion;
    try {
      unsigned char *p = image32.get();
      // const auto &imageee = lightfield->get_image_at(view_coordinate);
      // std::cout << imageee.get_channel_names().at(0) << std::endl;
      const auto &image = static_cast<const BT601Image<uint16_t> &>(
          lightfield->get_image_at(view_coordinate));

      auto rgb_image = convert::to<RGBImage>(image);

      const auto width = lightfield->get_views_width_u();
      const auto height = lightfield->get_views_height_v();

      for (auto i = decltype(height){0}; i < height; ++i) {
        for (auto j = decltype(width){0}; j < width; ++j) {
          const auto &[r, g, b] = rgb_image.get_pixel_at(i, j);
          *p++ = convert_from_16_bit_to_8_bit_using_depth<depth>(b);
          *p++ = convert_from_16_bit_to_8_bit_using_depth<depth>(g);
          *p++ = convert_from_16_bit_to_8_bit_using_depth<depth>(r);
          p++;
        }
      }

    } catch (ImageChannelExceptions::InvalidIndexReadException &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    };
  }
};


int main(int argc, char const *argv[]) {
  auto cli_configuration =
      LightfieldVisualizationConfiguration(argc, const_cast<char **>(argv));
  if (cli_configuration.is_help_mode()) {
    exit(0);
  }


  //! [Full LightfieldFromFile instantiation using a LightfieldIOConfiguration]
  LightfieldDimension<std::size_t> size(
      cli_configuration.get_t(), cli_configuration.get_s(), 32, 32);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
  LightfieldIOConfiguration configuration(
      cli_configuration.get_input_filename(), initial, size);

  //! [Instantiating a LightfieldFromFile using a LightfieldIOConfiguration]
  auto lightfield =
      std::make_unique<LightfieldFromFile<uint16_t>>(configuration);
  //! [Instantiating a LightfieldFromFile using a LightfieldIOConfiguration]
  //! [Full LightfieldFromFile instantiation using a LightfieldIOConfiguration]


  //! [Setting a view_io_policy into a Lightfield]
  auto policy = std::make_unique<ViewIOPolicyLimitedMemory<uint16_t>>();
  policy->set_max_bytes(lightfield->get_views_width_u() *
                        lightfield->get_views_height_v() * 2 * 100);
  lightfield->set_view_io_policy(std::move(policy));
  //! [Setting a view_io_policy into a Lightfield]


  auto window_with_lightfield = LightfieldAtX11Window<uint16_t>(
      cli_configuration.get_input_filename(), lightfield.get());

  window_with_lightfield.loop();

  return 0;
}