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
  // TODO: load from somewhere
  static constexpr const char *model_version = "0.1.1";
  static constexpr const char *view_version = "0.1.1";

  XPROPERTY(std::string, D, _data_str);
  XPROPERTY(std::string, D, _data_type);
  XPROPERTY(int, D, _width, default_width);
  XPROPERTY(int, D, _height, default_height);
  XPROPERTY(bool, D, _is_new_stream, true);

  inline void sync(const std::string &stream, bool is_new = true) {
    auto offset = stream.find('\n');
    if (offset == -1) {
      fprintf(stderr, "invalid stream: %s\n", stream.c_str());
      throw std::runtime_error("invalid stream");
    }
    _data_type = stream.substr(0, offset);
    _data_str = stream.substr(offset + 1);
    _is_new_stream = is_new;
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

    xw::xwidgets_serialize(_data_str(), state["_data_str"], buffers);
    xw::xwidgets_serialize(_data_type(), state["_data_type"], buffers);
    xw::xwidgets_serialize(_width(), state["_width"], buffers);
    xw::xwidgets_serialize(_height(), state["_height"], buffers);
    xw::xwidgets_serialize(_is_new_stream(), state["_is_new_stream"], buffers);
  }

  inline void apply_patch(const nl::json &patch,
                          const xeus::buffer_sequence &buffers) {
    base_type::apply_patch(patch, buffers);

    set_property_from_patch(_data_str, patch, buffers);
    set_property_from_patch(_data_type, patch, buffers);
    set_property_from_patch(_width, patch, buffers);
    set_property_from_patch(_height, patch, buffers);
    set_property_from_patch(_is_new_stream, patch, buffers);
  }

  inline void resize(int width = default_width, int height = default_height) {
    _width = width;
    _height = height;
  }

  // `display' conflicts with something in one of the parent classes
  // or xmaterialize
  inline void show(const std::string &stream) { sync(stream, true); }

  inline void update(const std::string &stream) { sync(stream, false); }
};

using glvis = xw::xmaterialize<xglvis>;
}; // namespace glvis

#endif // XGLVIS_HPP_
