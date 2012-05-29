//LIC// ====================================================================
//LIC// This file forms part of oomph-lib, the object-oriented, 
//LIC// multi-physics finite-element library, available 
//LIC// at http://www.oomph-lib.org.
//LIC// 
//LIC//           Version 0.90. August 3, 2009.
//LIC// 
//LIC// Copyright (C) 2006-2009 Matthias Heil and Andrew Hazel
//LIC// 
//LIC// This library is free software; you can redistribute it and/or
//LIC// modify it under the terms of the GNU Lesser General Public
//LIC// License as published by the Free Software Foundation; either
//LIC// version 2.1 of the License, or (at your option) any later version.
//LIC// 
//LIC// This library is distributed in the hope that it will be useful,
//LIC// but WITHOUT ANY WARRANTY; without even the implied warranty of
//LIC// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//LIC// Lesser General Public License for more details.
//LIC// 
//LIC// You should have received a copy of the GNU Lesser General Public
//LIC// License along with this library; if not, write to the Free Software
//LIC// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//LIC// 02110-1301  USA.
//LIC// 
//LIC// The authors may be contacted at oomph-lib@maths.man.ac.uk.
//LIC// 
//LIC//====================================================================
//Header file for a class that is used to represent a mesh
//as a geometric object

//Include guards to prevent multiple inclusion of the header
#ifndef OOMPH_MESH_AS_GEOMETRIC_OBJECT_HEADER
#define OOMPH_MESH_AS_GEOMETRIC_OBJECT_HEADER

// Config header generated by autoconfig
#ifdef HAVE_CONFIG_H
  #include <oomph-lib-config.h>
#endif

#include<limits.h>
#include<float.h>

//Include the geometric object header file
#include "geom_objects.h"

namespace oomph
{

//========================================================================
/// This class provides a GeomObject representation of a given
/// finite element mesh. The Lagrangian coordinate is taken to be the 
/// dimension of the (first) element in the mesh and the Eulerian 
/// coordinate is taken to be the dimension of the (first) node in 
/// the mesh. If there are no elements or nodes the appropriate dimensions
/// will be set to zero.
/// The consitituent elements of the mesh must have their own
/// GeomObject representations, so they must be FiniteElements,
///  and they become sub-objects
/// in this compound GeomObject.
//========================================================================
class MeshAsGeomObject : public GeomObject
{

private:

 /// \short  Helper function for constructor: pass the pointer to the mesh, 
 /// communicator and boolean
 /// to specify whether to calculate coordinate extrema or not.
 void construct_it(Mesh* const &mesh_pt, OomphCommunicator* comm_pt,
                   const bool& compute_extreme_bin_coords);

 /// \short Sort the sampling points in the specified bin by distance from 
 /// sampling point
 void sort_the_bin(const Vector<double>& zeta,
                   Vector<std::pair<FiniteElement*,Vector<double> > >&
                   sample_point_pairs);
 
 /// \short Vector of pointers to Data items that affects the object's shape
 Vector<Data*> Geom_data_pt;

 ///Internal storage for the elements that constitute the object
 Vector<FiniteElement*> Sub_geom_object_pt;

 ///Storage for paired objects and coords in each bin 
 Vector<Vector<std::pair<FiniteElement*,Vector<double> > > > 
  Bin_object_coord_pairs;

 /// \short In parallel computation, suppress synchronisation of bins
 /// Default is false. If set to true, each processor will create
 /// its own bin structure, spanning only its own elements
 bool Suppress_synchronisation_of_bins;

 /// \short Max radius beyond which we stop searching the bin. Initialised
 /// to DBL_MAX so keep going until the point is found or until
 /// we've searched every single bin. Overwriting this means we won't search
 /// in bins whose closest vertex is at a distance greater than
 /// Max_search_radius from the point to be located.
 double Max_search_radius;
  
 ///Storage for min coordinates in the mesh
 Vector<double> Min_coords;

 ///Storage for max coordinates in the mesh
 Vector<double> Max_coords;

 ///Number of bins in x direction
 unsigned Nbin_x;

 ///Number of bins in y direction
 unsigned Nbin_y;

 ///Number of bins in z direction
 unsigned Nbin_z;

 ///Current min. spiralling level
 unsigned Current_min_spiral_level;

 ///Current max. spiralling level
 unsigned Current_max_spiral_level;

 ///Communicator
 OomphCommunicator* Communicator_pt;

public:
 
 ///\short Constructor, pass the pointer to the mesh. 
 /// Final flag (default 0) allows
 /// the suppression of synchronisation of bin boundaries on multiple
 /// processors so if set to 1 each processor only creates a bin 
 /// structure that contains its own elements, rather than a bin
 /// bin structure that is identical for each processor. Obviously
 /// only relevant for distributed problems. 
 MeshAsGeomObject(Mesh* const &mesh_pt,
                  const unsigned& suppress_synchronisation_of_bins_flag=0) 
  : GeomObject()
  {
   Max_search_radius=DBL_MAX;
   Suppress_synchronisation_of_bins=false;
   if (suppress_synchronisation_of_bins_flag==1) 
    {
     Suppress_synchronisation_of_bins=true;
    }
   OomphCommunicator* comm_pt=0;
   bool compute_extreme_bin_coords=true;
   this->construct_it(mesh_pt,comm_pt,compute_extreme_bin_coords);
  }

 ///\short Constructor, pass the pointer to the mesh and communicator. 
 /// Final flag (default 0) allows
 /// the suppression of synchronisation of bin boundaries on multiple
 /// processors so if set to 1 each processor only creates a bin 
 /// structure that contains its own elements, rather than a bin
 /// bin structure that is identical for each processor. Obviously
 /// only relevant for distributed problems. 
  MeshAsGeomObject(Mesh* const &mesh_pt,
                   OomphCommunicator* comm_pt,
                   const unsigned& suppress_synchronisation_of_bins_flag=0) 
   : GeomObject()
  {
   Max_search_radius=DBL_MAX;
   Suppress_synchronisation_of_bins=false;
   if (suppress_synchronisation_of_bins_flag==1) 
    {
     Suppress_synchronisation_of_bins=true;
    }
   bool compute_extreme_bin_coords=true;
   this->construct_it(mesh_pt,comm_pt,compute_extreme_bin_coords);
  }

 ///\short Constructor, pass the pointer to the mesh and 
 /// boolean to bypass the computation of the extreme coordinates
 ///of the bin used in the locate_zeta method. Final flag (default 0) allows
 /// the suppression of synchronisation of bin boundaries on multiple
 /// processors so if set to 1 each processor only creates a bin 
 /// structure that contains its own elements, rather than a bin
 /// bin structure that is identical for each processor. Obviously
 /// only relevant for distributed problems. 
 MeshAsGeomObject(Mesh* const &mesh_pt,
                  const bool& compute_extreme_bin_coords,
                  const unsigned& suppress_synchronisation_of_bins_flag=0) : 
  GeomObject()
  {
   Max_search_radius=DBL_MAX;
   Suppress_synchronisation_of_bins=false;
   if (suppress_synchronisation_of_bins_flag==1) 
    {
     Suppress_synchronisation_of_bins=true;
    }
   OomphCommunicator* comm_pt=0;
   this->construct_it(mesh_pt,comm_pt,compute_extreme_bin_coords);
  }


 /// \short Constructor, pass the pointer to the mesh, communicator, and 
 /// boolean to bypass the computation of the extreme coordinates
 /// of the bin used in the locate_zeta method. Final flag (default 0) allows
 /// the suppression of synchronisation of bin boundaries on multiple
 /// processors so if set to 1 each processor only creates a bin 
 /// structure that contains its own elements, rather than a bin
 /// bin structure that is identical for each processor. Obviously
 /// only relevant for distributed problems. 
 MeshAsGeomObject(Mesh* const &mesh_pt,
                  OomphCommunicator* comm_pt,
                  const bool& compute_extreme_bin_coords,
                  const unsigned& suppress_synchronisation_of_bins_flag=0) : 
  GeomObject()
  {
   Max_search_radius=DBL_MAX;
   if (suppress_synchronisation_of_bins_flag==1) 
    {
     Suppress_synchronisation_of_bins=true;
    }
   Suppress_synchronisation_of_bins=false;
   this->construct_it(mesh_pt,comm_pt,compute_extreme_bin_coords);
  }
 
 /// Empty Constructor
 MeshAsGeomObject(){} 

 /// Destructor
 ~MeshAsGeomObject()
  {
   // Flush bin (mainly to decrement counter for number of 
   // active bins)
   flush_bins_of_objects();
  } 

 /// Broken copy constructor
 MeshAsGeomObject(const MeshAsGeomObject&) 
  { 
   BrokenCopy::broken_copy("MeshAsGeomObject");
  } 
 
 /// Broken assignment operator
 void operator=(const MeshAsGeomObject&) 
  {
   BrokenCopy::broken_assign("MeshAsGeomObject");
  }

 /// How many items of Data does the shape of the object depend on?
 unsigned ngeom_data() const {return Geom_data_pt.size();}
 
 /// \short Return pointer to the j-th Data item that the object's 
 /// shape depends on 
 Data* geom_data_pt(const unsigned& j) {return Geom_data_pt[j];}

 /// \short Find the sub geometric object and local coordinate therein that
 /// corresponds to the intrinsic coordinate zeta. If sub_geom_object_pt=0
 /// on return from this function, none of the constituent sub-objects 
 /// contain the required coordinate. Following from the general
 /// interface to this function in GeomObjects,
 /// setting the optional bool argument to true means that each
 /// time the sub-object's locate_zeta function is called, the coordinate
 /// argument "s" is used as the initial guess. However, this doesn't
 /// make sense here and the argument is ignored (though a warning
 /// is issued when the code is compiled in PARANOID setting)
 void locate_zeta(const Vector<double>& zeta, 
                  GeomObject*& sub_geom_object_pt, 
                  Vector<double>& s,
                  const bool& use_coordinate_as_initial_guess=false)
 {
#ifdef PARANOID
  if (use_coordinate_as_initial_guess)
   {
    OomphLibWarning(
     "Ignoring the use_coordinate_as_initial_guess argument.",
     "MeshAsGeomObject::locate_zeta()",
     OOMPH_EXCEPTION_LOCATION);  
   }
#endif
   
  // Call locate zeta with spiraling switched off
  bool called_within_spiral=false;
  spiraling_locate_zeta(zeta,sub_geom_object_pt,s,called_within_spiral);
 }

/// \short Find the sub geometric object and local coordinate therein that
/// corresponds to the intrinsic coordinate zeta. If sub_geom_object_pt=0
/// on return from this function, none of the constituent sub-objects 
/// contain the required coordinate.
/// Setting the final bool argument to true means that we only search
/// for matching element within a a certain number of "spirals" within
/// the bin structure.
 void spiraling_locate_zeta(const Vector<double>& zeta,
                            GeomObject*& sub_geom_object_pt,
                            Vector<double>& s, 
                            const bool &called_within_spiral);

 /// \short Return the position as a function of the intrinsic coordinate zeta.
 /// This provides an (expensive!) default implementation in which
 /// we loop over all the constituent sub-objects and check if they
 /// contain zeta and then evaluate their position() function.
 void position(const Vector<double> &zeta, Vector<double> &r) const
  {
   // Call position function at current timestep:
   unsigned t=0;
   position(t,zeta,r);
  }


 /// \short Parametrised position on object: r(zeta). Evaluated at
 /// previous timestep. t=0: current time; t>0: previous
 /// timestep. This provides an (expensive!) default implementation in which
 /// we loop over all the constituent sub-objects and check if they
 /// contain zeta and then evaluate their position() function. 
 void position(const unsigned& t, const Vector<double>& zeta,
               Vector<double>& r) const
  {
   // Storage for the GeomObject that contains the zeta coordinate
   // and the intrinsic coordinate within it.
   GeomObject* sub_geom_object_pt;
   const unsigned n_lagrangian = this->nlagrangian();
   Vector<double> s(n_lagrangian);

   //Find the sub object containing zeta, and the local intrinsic coordinate
   //within it
   const_cast<MeshAsGeomObject*>(this)->locate_zeta(zeta,sub_geom_object_pt,s);
   if(sub_geom_object_pt == 0) 
    {
     std::ostringstream error_message;
     error_message << "Cannot locate zeta ";
     for(unsigned i=0;i<n_lagrangian;i++)
      {
       error_message << zeta[i] << " ";
      }
     error_message << std::endl;
     throw OomphLibError(error_message.str(),
                         "MeshAsGeomObject::position()",
                         OOMPH_EXCEPTION_LOCATION);
    }
   //Call that sub-object's position function
   sub_geom_object_pt->position(t,s,r);

  } // end of position

 ///Return the derivative of the position
 void dposition(const Vector<double> &xi, DenseMatrix<double> &drdxi) const
  {
   throw OomphLibError("dposition() not implemented",
                       "MeshAsGeomObject::dposition()",
                       OOMPH_EXCEPTION_LOCATION);
  }


 /// \short Set maximum search radius for locate zeta. This is initialised
 /// do DBL_MAX so we brutally search through the entire bin structure,
 /// no matter how big it is until we've found the required point (or 
 /// failed to do so. This can be VERY costly with fine meshes.
 /// Here the user takes full responsibility and states that we have
 /// no chance in hell to find the required point in
 /// a bin whose closest vertex is further than the specified
 /// max search radius.
 double& max_search_radius() {return Max_search_radius;}

 ///Access function to current min. spiral level
 unsigned& current_min_spiral_level() {return Current_min_spiral_level;}

 ///Access function to current max. spiral level
 unsigned& current_max_spiral_level() {return Current_max_spiral_level;}

 ///Access function for min coordinate in x direction
 double& x_min() {return Min_coords[0];}

 ///Access function for max coordinate in x direction
 double& x_max() {return Max_coords[0];}

 ///Access function for min coordinate in y direction
 double& y_min() {return Min_coords[1];}

 ///Access function for max coordinate in y direction
 double& y_max() {return Max_coords[1];}

 ///Access function for min coordinate in z direction
 double& z_min() {return Min_coords[2];}

 ///Access function for max coordinate in z direction
 double& z_max() {return Max_coords[2];}

 ///Get the min and max coordinates for the mesh, in each dimension
 void get_min_and_max_coordinates(Mesh* const &mesh_pt);

 ///Number of bins in x direction
 unsigned nbin_x(){return Nbin_x;}

 ///Number of bins in y direction
 unsigned nbin_y(){return Nbin_y;}

 ///Number of bins in z direction
 unsigned nbin_z(){return Nbin_z;}

 /// \short Compute the minimum distance of any vertex in the specified bin
 /// from the specified Lagrangian coordinate zeta
 double min_distance(const unsigned& i_bin,
                     const Vector<double>& zeta);
 

 /// \short Output bin vertices (allowing display of bins as zones).
 /// Final argument specifies the coordinates of a point (defaults to
 /// zero vector) and output includes the minimum distance of any of
 /// the bin vertices to this point. 
 void output_bin_vertices(std::ofstream& outfile,
                          const Vector<double>& zeta);
 
 /// \short Output bin vertices (allowing display of bins as zones).
 void output_bin_vertices(std::ofstream& outfile)
 {
  Vector<double> zeta( this->nlagrangian(),0.0);
  output_bin_vertices(outfile,zeta);
 }
  
 /// \short Get vector of vectors containing the coordinates of the
 /// vertices of the i_bin-th bin: bin_vertex[j][i] contains the
 /// i-th coordinate of the j-th vertex.
 void get_bin_vertices(const unsigned& i_bin,
                       Vector<Vector<double> >& bin_vertex);
 
 /// Initialise and populate the "bin" structure for locating coordinates
 /// and increment counter for total number of bins in active use by any 
 /// MeshAsGeomObject)
 void create_bins_of_objects();

 /// \short Flush the storage for the binning method (and decrement counter
 /// for total number of bins in active use by any MeshAsGeomObject)
 void flush_bins_of_objects()
  {
   Total_nbin_cells_counter-=Bin_object_coord_pairs.size();
   Bin_object_coord_pairs.clear();
  }

 /// \short Get the number of the bin containing the specified coordinate.
 /// Bin number is negative if the coordinate is outside
 /// the bin structure.
 void get_bin(const Vector<double>& zeta, int& bin_number);
 
 /// \short Get the number of the bin containing the specified coordinate; also
 /// return the contents of that bin. Bin number is negative if the 
 /// coordinate is outside the bin structure.
 void get_bin(const Vector<double>& zeta, int& bin_number,
              Vector<std::pair<FiniteElement*,
              Vector<double> > >& sample_point_pairs);
 
 /// Get the contents of the specified bin
 Vector<std::pair<FiniteElement*,Vector<double> > >& bin_content(
  const unsigned& bin_number);
 
 ///Calculate the bin numbers of all the neighbours to "bin" given the level
 void get_neighbouring_bins_helper(const unsigned& bin_number, 
                                   const unsigned& level,
                                   Vector<unsigned>& neighbour_bin);

 /// \short Fill bin by diffusion, populating each empty bin with the 
 /// same content as the first non-empty bin found during a spiral-based search 
 /// up to the specified "radius" (default 1)
 void fill_bin_by_diffusion(const unsigned& bin_diffusion_radius=1);

 /// Output bins
 void output_bins(std::ofstream& outfile);

 /// Output bins
 void output_bins(std::string& filename)
 {
  std::ofstream outfile;
  outfile.open(filename.c_str());
  output_bins(outfile);
  outfile.close();
 }


 /// \short Counter for overall number of bins allocated -- used to
 /// issue warning if this exceeds a threshhold. (Default assignment
 /// of 100^DIM bins per MeshAsGeomObject can be a killer if there
 /// are huge numbers of sub-meshes (e.g. in unstructured FSI).
 static unsigned long Total_nbin_cells_counter;

 /// \short Total number of bins above which warning is issued.
 /// (Default assignment of 100^DIM bins per MeshAsGeomObject can 
 /// be a killer if there are huge numbers of sub-meshes (e.g. in 
 /// unstructured FSI).
 static unsigned long Threshold_for_total_bin_cell_number_warning;

 /// \short Boolean to supppress warnings about large number of bins
 static bool Suppress_warning_about_large_total_number_of_bins;

 /// \short Boolean flag to make sure that warning about large number
 /// of bin cells only gets triggered once.
 static bool Already_warned_about_large_number_of_bin_cells;

};

}

#endif
