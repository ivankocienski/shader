
#pragma once

#include <cctype>
#include <vector>

class MeshLoader {
public:

  typedef unsigned short index_t;

private:

  std::vector<float>   m_vertex_data;
  std::vector<float>   m_normal_data;
  std::vector<index_t> m_index_data;

public:

  MeshLoader();

  void load_from_obj_file( const char* );

  size_t vertex_byte_size() const;
  size_t index_byte_size() const;
  size_t normal_byte_size() const;

  size_t vertex_count() const;
  size_t index_count() const;
  size_t normal_count() const;

  const float* vertex_ptr() const;
  const index_t* index_ptr() const;
  const float* normal_ptr() const;
};

