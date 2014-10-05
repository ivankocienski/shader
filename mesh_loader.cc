
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
  regex normal_r( "^vn\\s+(\\S+)\\s+(\\S+)\\s+(\\S+).*$" );
  regex index_r(  "^f\\s+(\\S+)\\s+(\\S+)\\s+(\\S+).*$" );

  ifstream file(fp);
  string line;

  int vert_count = 0;
  int ind_count  = 0;
  int norm_count = 0;

  while( !file.eof() ) {
    getline( file, line );

    if( regex_match( line, capture, vertex_r )) { 

      m_vertex_data.push_back( lexical_cast<float>(capture[1]) );
      m_vertex_data.push_back( lexical_cast<float>(capture[2]) );
      m_vertex_data.push_back( lexical_cast<float>(capture[3]) ); 

      vert_count++;
    }

    if( regex_match( line, capture, normal_r )) { 

      m_normal_data.push_back( lexical_cast<float>(capture[1]) );
      m_normal_data.push_back( lexical_cast<float>(capture[2]) );
      m_normal_data.push_back( lexical_cast<float>(capture[3]) ); 

      norm_count++;
    }

    if( regex_match( line, capture, index_r )) {
      m_index_data.push_back( lexical_cast<index_t>(capture[1]) - 1 );
      m_index_data.push_back( lexical_cast<index_t>(capture[2]) - 1 );
      m_index_data.push_back( lexical_cast<index_t>(capture[3]) - 1 ); 

      ind_count++;
    }

  }

  cout << "from " << fp << endl;
  cout << "  vert_count=" << vert_count << endl;
  cout << "  ind_count="  << ind_count  << endl;
  cout << "  norm_count=" << norm_count << endl;

  cout << "  vertex.size=" << m_vertex_data.size() << endl;
  cout << "  index.size="  << m_index_data.size()  << endl;
  cout << "  normal.size=" << m_normal_data.size() << endl;

}

size_t MeshLoader::data_byte_size() const {
  return m_vertex_data.size() * sizeof(node_t);
}

//size_t MeshLoader::vertex_byte_size() const {
//  return m_vertex_data.size() * sizeof(float);
//}

size_t MeshLoader::index_byte_size() const {
  return m_index_data.size() * sizeof(index_t);
}

//size_t MeshLoader::normal_byte_size() const {
//  return m_normal_data.size() * sizeof(float);
//}

size_t MeshLoader::vertex_count() const {
  return m_vertex_data.size() / 3;
}

size_t MeshLoader::index_count() const {
  return m_index_data.size() / 3;
}

//size_t MeshLoader::normal_count() const {
//  return m_normal_data.size() / 3;
//}

//const float *MeshLoader::vertex_ptr() const {
//  return m_vertex_data.data();
//}

const MeshLoader::index_t* MeshLoader::index_ptr() const {
  return m_index_data.data();
}

//const float *MeshLoader::normal_ptr() const {
//  return m_normal_data.data();
//}

void MeshLoader::write_vertex_data_to_buffer( node_p vertex_buffer ) const {

  int count  = m_vertex_data.size();
  int offset = 0;

  vector<float>::const_iterator v_it = m_vertex_data.begin();
  vector<float>::const_iterator n_it = m_normal_data.begin();

  while( count ) {

    vertex_buffer->position[offset] = *v_it;
    vertex_buffer->normal[offset]   = *n_it;

    offset++;
    if( offset > 2 ) {
      offset = 0;
      vertex_buffer++;
    }

    n_it++;
    v_it++;
    count--;
  }

}



