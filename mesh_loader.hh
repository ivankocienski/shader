
#pragma once

#include <cctype>
#include <vector>

class MeshLoader {
public:

  typedef unsigned short index_t;

  typedef struct _node_s {
    float position[3];
    float normal[3];
    char _padding[2];
  } node_t, *node_p;


private:

  std::vector<float>   m_vertex_data;
  std::vector<float>   m_normal_data;
  std::vector<index_t> m_index_data;

public:

  MeshLoader();

  void load_from_obj_file( const char* );

  size_t index_byte_size() const;
  size_t index_count() const;
  const index_t* index_ptr() const;

  size_t data_byte_size() const;
  size_t vertex_count() const;
  void write_vertex_data_to_buffer( node_p ) const;
};

