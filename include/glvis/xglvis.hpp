// Copyright(c) 2010 - 2021, Lawrence Livermore National Security, LLC.Produced
// at the Lawrence Livermore National Laboratory.All Rights reserved.See files
// LICENSE and NOTICE for details.LLNL - CODE - 443271.
//
// This file is part of the GLVis visualization tool and library.For more
// information and source code availability see https: // glvis.org.
//
// GLVis is free software; you can redistribute it and / or modify it under the
// terms of the BSD - 3 license.We welcome feedback and contributions, see file
// CONTRIBUTING.md for details.

#ifndef XGLVIS_HPP_
#define XGLVIS_HPP_

#include <fstream>
#include <stdexcept>
#include <string>

#include <xwidgets/xcommon.hpp>
#include <xwidgets/xmaterialize.hpp>
#include <xwidgets/xwidget.hpp>

namespace glvis {
std::string read_file(const std::string &name) {
  std::ifstream f(name);
  if (!f) {
    fprintf(stderr, "file does not exist: %s\n", name.c_str());
    throw std::runtime_error("invalid path");
  }

  std::string str;
  f.seekg(0, std::ios::end);
  str.reserve(f.tellg());
  f.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(f)),
             std::istreambuf_iterator<char>());

  return str;
}

template <class D> class xglvis : public xw::xwidget<D> {
private:
  using base_type = xw::xwidget<D>;
  static constexpr int default_width = 640;
  static constexpr int default_height = 480;
  static constexpr const char *model_version = "0.3.1";
  static constexpr const char *view_version = "0.3.1";

  XPROPERTY(std::string, D, data_str);
  XPROPERTY(std::string, D, data_type);
  XPROPERTY(int, D, width, default_width);
  XPROPERTY(int, D, height, default_height);
  XPROPERTY(bool, D, is_new_stream, true);

  inline void sync(const std::string &stream, bool is_new = true) {
    auto offset = stream.find('\n');
    if (offset == -1) {
      fprintf(stderr, "invalid stream: %s\n", stream.c_str());
      throw std::runtime_error("invalid stream");
    }
    data_type = stream.substr(0, offset);
    data_str = stream.substr(offset + 1);
    is_new_stream = is_new;
  }

  inline void set_defaults() {
    this->_model_module() = "glvis-jupyter";
    this->_view_module() = "glvis-jupyter";
    this->_model_name() = "GLVisModel";
    this->_view_name() = "GLVisView";
    this->_model_module_version() = std::string("^") + model_version;
    this->_view_module_version() = std::string("^") + view_version;
  }

public:
  xglvis(/*const std::string & stream*/) : base_type() {
    set_defaults();
    // show(stream);
  }

  inline void serialize_state(nl::json &state,
                              xeus::buffer_sequence &buffers) const {
    base_type::serialize_state(state, buffers);

    xw::xwidgets_serialize(data_str(), state["data_str"], buffers);
    xw::xwidgets_serialize(data_type(), state["data_type"], buffers);
    xw::xwidgets_serialize(width(), state["width"], buffers);
    xw::xwidgets_serialize(height(), state["height"], buffers);
    xw::xwidgets_serialize(is_new_stream(), state["is_new_stream"], buffers);
  }

  inline void apply_patch(const nl::json &patch,
                          const xeus::buffer_sequence &buffers) {
    base_type::apply_patch(patch, buffers);

    set_property_from_patch(data_str, patch, buffers);
    set_property_from_patch(data_type, patch, buffers);
    set_property_from_patch(width, patch, buffers);
    set_property_from_patch(height, patch, buffers);
    set_property_from_patch(is_new_stream, patch, buffers);
  }

  inline void set_size(int width = default_width, int height = default_height) {
    width = width;
    height = height;
  }

  inline void plot(const std::string &stream) { sync(stream, true); }

  inline void update(const std::string &stream) { sync(stream, false); }
};

using glvis = xw::xmaterialize<xglvis>;
}; // namespace glvis

#endif // XGLVIS_HPP_
