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
void init_x();
void close_x();
void redraw();

unsigned char *image32;
size_t past_positional_view_x = 5;
size_t past_positional_view_y = 5;
int inverted_axis = 0;
int horizontal_views = 0;
int vertical_views = 0;

std::string resources_path = "../resources";

LightfieldDimension<std::size_t> size(3, 3, 32, 32);
LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
LightfieldIOConfiguration configuration(
    {resources_path + "/small_greek/"}, initial, size);

std::unique_ptr<LightfieldFromPPMFile<uint16_t>> lightfield =
    std::make_unique<LightfieldFromPPMFile<uint16_t>>(configuration);

uint8_t convert_from_16_bit_to_8_bit_using_depth(
    uint16_t source, uint8_t depth) {
  uint32_t shift = depth - 8;
  return (source) >> shift;
}


void load_view_into_image(
    const std::pair<std::size_t, std::size_t> &view_coordinate) {
  try {
    auto &view_ptr = lightfield->get_view_at(view_coordinate);
    unsigned char *p = image32;

    for (auto i = decltype(view_ptr.get_width()){0}; i < view_ptr.get_width();
         i++) {
      for (auto j = decltype(view_ptr.get_height()){0};
           j < view_ptr.get_height(); j++) {
        const auto &[r, g, b] = view_ptr.get_pixel_at({i, j});
        *p++ = convert_from_16_bit_to_8_bit_using_depth(b, view_ptr.get_bpp());
        *p++ = convert_from_16_bit_to_8_bit_using_depth(g, view_ptr.get_bpp());
        *p++ = convert_from_16_bit_to_8_bit_using_depth(r, view_ptr.get_bpp());
        *p++ = 0;
      }
    }

  } catch (ImageChannelExceptions::InvalidIndexReadException &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  };
}


// int main(int argc, char const *argv[]) {
int main() {
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
  init_x();
  while (1) {
    XNextEvent(display, &event);

    if (event.type == Expose && event.xexpose.count == 0) {
      redraw();
    }
    if (event.type == KeyPress &&
        XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
      if (text[0] == 'q') {
        close_x();
      }
      printf("You pressed the %c key!\n", text[0]);
    }
    if (event.type == MotionNotify) {
      auto x = event.xbutton.x;
      auto y = event.xbutton.y;

      auto positional_view_x = x / view_relative_horizontal_size;
      auto positional_view_y = y / view_relative_vertical_size;

      auto t = static_cast<std::size_t>(std::round(positional_view_x));
      auto s = static_cast<std::size_t>(std::round(positional_view_y));

      if ((past_t != t) || (past_s != s)) {
        past_t = t;
        past_s = s;
        load_view_into_image({t, s});
        redraw();
      }
    }
  }

  return 0;
}

void init_x() {
  unsigned long black, white;

  display = XOpenDisplay((char *) 0);
  screen = DefaultScreen(display);
  black = BlackPixel(display, screen), white = WhitePixel(display, screen);

  load_view_into_image({0, 0});

  win = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0,
      lightfield->get_views_width(), lightfield->get_views_height(), 5, black,
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

void redraw() {
  XClearWindow(display, win);
  Visual *visual = DefaultVisual(display, 0);
  XImage *ximage =
      XCreateImage(display, visual, 24, ZPixmap, 0, (char *) image32,
          lightfield->get_views_width(), lightfield->get_views_height(), 32, 0);
  XPutImage(display, win, DefaultGC(display, 0), ximage, 0, 0, 0, 0,
      lightfield->get_views_width(), lightfield->get_views_height());
}
