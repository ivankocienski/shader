
#pragma once

#include <cctype>
#include <vector>

class MeshLoader {
private:

  std::vector<float> m_vertex_data;
  std::vector<int> m_index_data;

public:

  MeshLoader();

  void load_from_obj_file( const char* );

  size_t vertex_byte_size() const;
  size_t index_byte_size() const;

  size_t vertex_count() const;
  size_t index_count() const;

  const float* vertex_ptr() const;
  const int* index_ptr() const;
};

