#ifndef UTIL_PLY_HELPER_H
#define UTIL_PLY_HELPER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// #include "i23dSFM/numeric/numeric.h"
#include "Eigen/Core"
#include "Eigen/Dense"

#include "stlplus3/file_system.hpp"

namespace GraphSfM {
namespace plyHelper {

/// Export 3D point vector to PLY format
static bool ExportToPly(const std::vector<Eigen::Vector3d>& vec_points,
                        const std::string& sFileName)
{
    std::ofstream outfile;
    outfile.open(sFileName.c_str(), std::ios_base::out);

    outfile << "ply"
            << std::endl << "format ascii 1.0"
            << std::endl << "element vertex " << vec_points.size()
            << std::endl << "property float x"
            << std::endl << "property float y"
            << std::endl << "property float z"
            << std::endl << "property uchar red"
            << std::endl << "property uchar green"
            << std::endl << "property uchar blue"
            << std::endl << "end_header" << std::endl;

    for (size_t i=0; i < vec_points.size(); ++i) {
      outfile << vec_points[i].transpose()
              << " 255 255 255" << "\n";
    }

    bool bOk = outfile.good();
    outfile.close();
    return bOk;
}

/// Export 3D point vector and camera position to PLY format
static bool ExportToPly(const std::vector<Eigen::Vector3d> & vec_points,
                        const std::vector<Eigen::Vector3d> & vec_camPos,
                        const std::string& sFileName,
                        const std::vector<Eigen::Vector3d> * vec_coloredPoints = NULL)
{
    std::ofstream outfile;
    outfile.open(sFileName.c_str(), std::ios_base::out);

    outfile << "ply"
            << '\n' << "format ascii 1.0"
            << '\n' << "element vertex " << vec_points.size() // + vec_camPos.size()
            << '\n' << "property float x"
            << '\n' << "property float y"
            << '\n' << "property float z"
            << '\n' << "property uchar red"
            << '\n' << "property uchar green"
            << '\n' << "property uchar blue"
            << '\n' << "end_header" << std::endl;

    for (size_t i = 0; i < vec_points.size(); ++i)  {
        if (vec_coloredPoints == NULL)
            outfile << vec_points[i].transpose()
                    << " 255 255 255" << "\n";
      else
        outfile << vec_points[i].transpose()
                << " " << (*vec_coloredPoints)[i].transpose() << "\n";
    }
    outfile.flush();

    std::string folder = stlplus::folder_part(sFileName);
    std::string basename = stlplus::basename_part(sFileName);

    std::string camera_filename = folder + "/" + basename + "_poses.ply";
    std::ofstream camera_outfile(camera_filename);
    if (!camera_outfile.is_open()) {
        std::cout << camera_filename << " cannot be opened!\n";
        return false;
    }
    camera_outfile << "ply"
                   << '\n' << "format ascii 1.0"
                   << '\n' << "element vertex " << + vec_camPos.size()
                   << '\n' << "property float x"
                   << '\n' << "property float y"
                   << '\n' << "property float z"
                   << '\n' << "property uchar red"
                   << '\n' << "property uchar green"
                   << '\n' << "property uchar blue"
                   << '\n' << "end_header" << std::endl;
    for (size_t i = 0; i < vec_camPos.size(); ++i)  {
        camera_outfile << vec_camPos[i].transpose()
                       << " 0 255 0" << "\n";
    }
    camera_outfile.flush();
  
    bool bOk = outfile.good() && camera_outfile.good();
    outfile.close();
    camera_outfile.close();

    return bOk;
}

static bool ExportToPly(const std::vector<Eigen::Vector3d> & vec_points,
                        const std::vector<Eigen::Vector3d> * vec_coloredPoints,
                        const std::string & sFileName)
{
    std::ofstream outfile;
    outfile.open(sFileName.c_str(), std::ios_base::out);

    outfile << "ply"
      << '\n' << "format ascii 1.0"
      << '\n' << "element vertex " << vec_points.size()
      << '\n' << "property float x"
      << '\n' << "property float y"
      << '\n' << "property float z"
      << '\n' << "property uchar red"
      << '\n' << "property uchar green"
      << '\n' << "property uchar blue"
      << '\n' << "end_header" << std::endl;

    for (size_t i=0; i < vec_points.size(); ++i)  {
        if (vec_coloredPoints == NULL)
            outfile << vec_points[i].transpose()
              << " 255 255 255" << "\n";
        else
            outfile << vec_points[i].transpose()
              << " " << (*vec_coloredPoints)[i].transpose() << "\n";
    }

    outfile.flush();
    bool bOk = outfile.good();
    outfile.close();
    return bOk;
}

static bool ReadPly(std::vector<Eigen::Vector3d>& vec_points, 
                    const std::string filename, 
                    std::vector<Eigen::Vector3d>& vec_coloredPoints)
{
    std::ifstream infile(filename);
    if(infile.is_open()) {
        int i = 0;
        std::string line;
        while(i++ <= 9) getline(infile, line);

        double x, y, z, r, g, b;
        while(infile >> x >> y >> z >> r >> g >> b) {
          vec_points.push_back(Eigen::Vector3d(x, y, z));
          vec_coloredPoints.push_back(Eigen::Vector3d(r, g, b));
        }
    }
    else {
        std::cout << filename << " cannot be opened!\n";
        return false;
    }

    infile.close();
    return true;
}

} // namespace plyHelper
} // namespace GraphSfM

#endif // UTIL_PLY_HELPER_H

