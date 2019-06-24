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
#include "Lib/Part2/Common/LightfieldFromPPMFile.h"
//X11:
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
//used for x11
Display *display;
int screen;
Window win;
GC gc;
void init_x(Lightfield<uint16_t> &lightfield,
    const LightfieldIOConfiguration &configuration);
void close_x();
void redraw(const Lightfield<uint16_t> &lightfield);


unsigned char *image32;
size_t past_positional_view_x = 0;
size_t past_positional_view_y = 0;
int inverted_axis = 0;
int horizontal_views = 0;
int vertical_views = 0;


template<int depth = 10>
uint8_t convert_from_16_bit_to_8_bit_using_depth(uint16_t source) {
  constexpr auto shift = depth - 8;
  return (source) >> shift;
}

template<int depth = 10>
void load_view_into_image(Lightfield<uint16_t> &lightfield,
    const std::pair<std::size_t, std::size_t> &view_coordinate) {
  try {
    unsigned char *p = image32;
    const auto &image = static_cast<const RGBImage<uint16_t> &>(
        lightfield.get_image_at(view_coordinate));

    const auto width = lightfield.get_views_width();
    const auto height = lightfield.get_views_height();

    for (auto i = decltype(height){0}; i < height; ++i) {
      for (auto j = decltype(width){0}; j < width; ++j) {
        const auto &[r, g, b] = image.get_pixel_at(i, j);
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


std::string resources_path = "../resources";

/**
 * @brief      { function_description }
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv
 * @snippet    LightfieldVisualization.cpp Instantiating a LightfieldFromPPMFile using a LightfieldIOConfiguration
 * @return     { description_of_the_return_value }
 */
int main(int argc, char const *argv[]) {
  std::string path_to_lightfield;
  if (argc < 2) {
    path_to_lightfield = std::string({resources_path + "/small_greek/"});
  } else {
    path_to_lightfield = std::string(argv[1]);
  }

  int t = 3;
  int s = 3;

  if (argc >= 3) {
    t = s = atoi(argv[2]);
  }

   if (argc == 4) {
    s = atoi(argv[3]);
  }

  LightfieldDimension<std::size_t> size(t, s, 32, 32);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
  LightfieldIOConfiguration configuration(path_to_lightfield, initial, size);

  //! [Instantiating a LightfieldFromPPMFile using a LightfieldIOConfiguration]
  std::unique_ptr<LightfieldFromPPMFile<uint16_t>> lightfield =
      std::make_unique<LightfieldFromPPMFile<uint16_t>>(configuration);
  //! [Instantiating a LightfieldFromPPMFile using a LightfieldIOConfiguration]

  auto view_relative_horizontal_size =
      lightfield->get_views_width() / ((double) lightfield->get_width() - 1);
  auto view_relative_vertical_size =
      lightfield->get_views_height() / ((double) lightfield->get_height() - 1);

  XEvent event;
  KeySym key;

  std::size_t past_t = 0;
  std::size_t past_s = 0;

  image32 = (unsigned char *) malloc(
      lightfield->get_views_width() * lightfield->get_views_height() * 4);


  char text[255];

  // auto policy = std::make_unique<ViewIOPolicyOneAtATime<uint16_t>>();
  // auto policy = std::make_unique<ViewIOPolicyLimitedNumberOfViews<uint16_t>>();
  // auto policy = std::make_unique<ViewIOPolicyLimitedMemory<uint16_t>>();
  auto policy = std::make_unique<ViewIOPolicyLimitedMemory<uint16_t>>();
  policy->set_max_bytes(
      lightfield->get_views_width() * lightfield->get_views_height() * 2 * 100);
  // auto policy = std::make_unique<ViewIOPolicyLimitlessMemory<uint16_t>>();

  // auto policy = ViewIOPolicyOneAtATime<uint16_t>();
  lightfield->set_view_io_policy(std::move(policy));

  init_x(*lightfield, configuration);

  while (1) {
    auto pending = XPending(display);
    XNextEvent(display, &event);
    auto last_event = event;

    if (event.type == MotionNotify) {
      while(event.type == MotionNotify && pending > 1) {
        last_event = event;
        XNextEvent(display, &event);
        pending = XPending(display);        
      }
      auto x = event.xbutton.x;
      auto y = event.xbutton.y;

      auto positional_view_x = x / view_relative_horizontal_size;
      auto positional_view_y = y / view_relative_vertical_size;

      auto t = static_cast<std::size_t>(std::round(positional_view_x));
      auto s = static_cast<std::size_t>(std::round(positional_view_y));

      if ((past_t != t) || (past_s != s)) {
        past_t = t;
        past_s = s;
        load_view_into_image(*lightfield, {t, s});
        redraw(*lightfield);
      }
      continue;
    }
    if (event.type == Expose && event.xexpose.count == 0) {
      redraw(*lightfield);
      continue;
    }
    if (event.type == KeyPress &&
        XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
      if (text[0] == 'q') {
        close_x();
      }
      printf("You pressed the %c key!\n", text[0]);
    }
  }

  return 0;
}

void init_x(Lightfield<uint16_t> &lightfield,
    const LightfieldIOConfiguration &configuration) {
  unsigned long black, white;

  display = XOpenDisplay((char *) 0);
  screen = DefaultScreen(display);
  black = BlackPixel(display, screen), white = WhitePixel(display, screen);

  load_view_into_image(lightfield, {0, 0});

  win = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0,
      lightfield.get_views_width(), lightfield.get_views_height(), 5, black,
      white);

  XSetStandardProperties(display, win, configuration.get_path().c_str(), "Test",
      None, NULL, 0, NULL);

  XSelectInput(display, win,
      ExposureMask | ButtonPressMask | KeyPressMask | PointerMotionMask);
  gc = XCreateGC(display, win, 0, 0);
  XSetBackground(display, gc, white);
  XSetForeground(display, gc, black);
  XClearWindow(display, win);
  XMapRaised(display, win);
}

void close_x() {
  XFreeGC(display, gc);
  XDestroyWindow(display, win);
  XCloseDisplay(display);
  exit(1);
}

void redraw(const Lightfield<uint16_t> &lightfield) {
  XClearWindow(display, win);
  Visual *visual = DefaultVisual(display, 0);
  XImage *ximage =
      XCreateImage(display, visual, 24, ZPixmap, 0, (char *) image32,
          lightfield.get_views_width(), lightfield.get_views_height(), 32, 0);
  XPutImage(display, win, DefaultGC(display, 0), ximage, 0, 0, 0, 0,
      lightfield.get_views_width(), lightfield.get_views_height());
}
