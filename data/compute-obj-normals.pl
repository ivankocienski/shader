#!/usr/bin/perl

use strict;
use warnings;

use Data::Dumper;

# A: vertex list as loaded from OBJ
my $vertex_list      = [];

# B: triangle list as loaded from OBJ
my $triangle_list    = [];

# C: the normals of the triangles B
my $normal_list      = [];

# D: which vertices A belong to which triangles B
my $vertex_triangles = [];

# E: calculated normals for each vertex A
my $vertex_normals   = [];

sub load_file {
  my $filename = shift;
  my $fh;

  open( $fh, $filename ) or die $!;

  while( my $line = <$fh> ) {
    
    if( $line =~ /^v\s+(\S+)\s+(\S+)\s+(\S+)/ ) {
      my $vertex = {
        x => $1 + 0.0,
        y => $2 + 0.0,
        z => $3 + 0.0
      };

      push $vertex_list, $vertex;
    }

    if( $line =~ /^f\s+(\S+)\s+(\S+)\s+(\S+)/ ) {
      my $triangle = {
        a => int($1) - 1,
        b => int($2) - 1,
        c => int($3) - 1
      };

      push $triangle_list, $triangle;
    }
  }

  close( $fh );

  my $vs = scalar @$vertex_list;
  my $fs = scalar @$triangle_list;
  print "v_count=$vs\n";
  print "f_count=$fs\n";
}

sub compute_triangle_normals {

  #my $ts = scalar @$triangle_list;
  #print "ts=$ts\n";

  for my $triangle (@$triangle_list) {

    #print "vc=$triangle->{c}\n";

    my $va = $vertex_list->[$triangle->{a}];
    my $vb = $vertex_list->[$triangle->{b}];
    my $vc = $vertex_list->[$triangle->{c}];


    #print Dumper( $vc );


    my $v1x = $vb->{x} - $va->{x};
    my $v1y = $vb->{y} - $va->{y};
    my $v1z = $vb->{z} - $va->{z};

    my $v2x = $vc->{x} - $va->{x};
    my $v2y = $vc->{y} - $va->{y};
    my $v2z = $vc->{z} - $va->{z};

    # compute normals...
    my $nx = ($v1y * $v2z) - ($v1z * $v2y);
    my $ny = ($v1z * $v2x) - ($v1x * $v2z);
    my $nz = ($v1x * $v2y) - ($v1y * $v2x);

    my $mag = sqrt( $nx * $nx + $ny * $ny + $nz * $nz );

    $nx /= $mag;
    $ny /= $mag;
    $nz /= $mag;

    my $normal = {
      x => $nx,
      y => $ny,
      z => $nz
    };

    push $normal_list, $normal;

  }

}

sub gather_vertex_triangles {

  # for each vertex, gather the triangles it belongs to...

  my $triangle_id = 0;

  for my $triangle (@$triangle_list) {

    my $va = $triangle->{a};
    my $vb = $triangle->{b};
    my $vc = $triangle->{c};

    $vertex_triangles->[$va] ||= [];
    push $vertex_triangles->[$va], $triangle_id;

    $vertex_triangles->[$vb] ||= [];
    push $vertex_triangles->[$vb], $triangle_id;

    $vertex_triangles->[$vc] ||= [];
    push $vertex_triangles->[$vc], $triangle_id;

    $triangle_id++;
  }
}

sub average_vertex_normals {

  for my $vt (@$vertex_triangles ) {

    next if !$vt;

    my $count = 0;
    my $sum_x = 0.0;
    my $sum_y = 0.0;
    my $sum_z = 0.0;

    for my $triangle_id (@$vt) {
      $sum_x += ($normal_list->[ $triangle_id ])->{x};
      $sum_y += ($normal_list->[ $triangle_id ])->{y};
      $sum_z += ($normal_list->[ $triangle_id ])->{z};
      
      $count++;
    }

    $sum_x /= $count;
    $sum_y /= $count;
    $sum_z /= $count;

    my $vn = {
      x => $sum_x,
      y => $sum_y,
      z => $sum_z
    };

    push $vertex_normals, $vn; 
  }

  my $s = scalar @$vertex_normals;
}

sub save_file {
  my $filename = shift; 
  my $fh;

  open( $fh, '>', $filename ) or dir $!;

  print $fh "# object file\n\n";

  for my $vertex (@$vertex_list) {
    my $x = $vertex->{x};
    my $y = $vertex->{y};
    my $z = $vertex->{z};

    printf $fh "v %.4f %.4f %4f\n", $x, $y, $z;
  }

  print $fh "\n\n\n";

  for my $vertex_normal (@$vertex_normals) {
    my $x = $vertex_normal->{x};
    my $y = $vertex_normal->{y};
    my $z = $vertex_normal->{z};

    printf $fh "vn %.4f %.4f %4f\n", $x, $y, $z;
  }

  print $fh "\n\n\n";

  for my $triangle (@$triangle_list) {
    my $a = $triangle->{a} + 1;
    my $b = $triangle->{b} + 1;
    my $c = $triangle->{c} + 1;

    printf $fh "f %d %d %d\n", $a, $b, $c;
  }

  close $fh;
}

# load file
load_file( "bunny.obj" );

# compute normals
compute_triangle_normals();

# TODO: smooth normals !
gather_vertex_triangles();

# average the vertex triangles...
average_vertex_normals();

# condense normals?

# write
save_file( "bunny-normalized.obj" );

