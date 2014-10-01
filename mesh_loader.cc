
#include <string>
#include <fstream>
#include <vector>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "mesh_loader.hh"

using namespace std;
using namespace boost;

MeshLoader::MeshLoader() { }

void MeshLoader::load_from_obj_file( const char *fp ) {

  smatch capture;
  regex vertex_r( "^v\\s+(\\S+)\\s+(\\S+)\\s+(\\S+).*$" );
  regex index_r(  "^f\\s+(\\S+)\\s+(\\S+)\\s+(\\S+).*$" );

  ifstream file(fp);
  string line;
  int vert_count = 0;
  int ind_count = 0;

  while( !file.eof() ) {
    getline( file, line );

    if( regex_match( line, capture, vertex_r )) { 

      m_vertex_data.push_back( lexical_cast<float>(capture[1]) );
      m_vertex_data.push_back( lexical_cast<float>(capture[2]) );
      m_vertex_data.push_back( lexical_cast<float>(capture[3]) ); 

      vert_count++;
    }

    if( regex_match( line, capture, index_r )) {
      m_index_data.push_back( lexical_cast<int>(capture[1]) );
      m_index_data.push_back( lexical_cast<int>(capture[2]) );
      m_index_data.push_back( lexical_cast<int>(capture[3]) ); 

      ind_count++;
    }

  }

  cout << "from " << fp << endl;
  cout << "  vert_count=" << vert_count << endl;
  cout << "  ind_count=" << ind_count << endl;

  cout << "  vertex.size=" << m_vertex_data.size() << endl;
  cout << "  index.size=" << m_index_data.size() << endl;

}

size_t MeshLoader::vertex_byte_size() const {
  return m_vertex_data.size() * sizeof(float);
}

size_t MeshLoader::index_byte_size() const {
  return m_index_data.size() * sizeof(float);
}

size_t MeshLoader::vertex_count() const {
  return m_vertex_data.size() / 3;
}

size_t MeshLoader::index_count() const {
  return m_index_data.size() / 3;
}

const float *MeshLoader::vertex_ptr() const {
  return m_vertex_data.data();
}

const int* MeshLoader::index_ptr() const {
  return m_index_data.data();
}
