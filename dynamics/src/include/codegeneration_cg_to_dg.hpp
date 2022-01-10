//------------------------------------
#ifndef __codegeneration_cg2dg__
#define __codegeneration_cg2dg__
//------------------------------------

// Automatically generated by codegeneration/project_cg_dg.py
//
// Generates the matrices CG2toDG[dg]
// - Realizes the projection of a CG2 vector into the DG[dg] space
//   dg = CG2toDG[dg] * cg

//------------------------------ CG2toDG

static const Eigen::Matrix<double, 3, 9, Eigen::RowMajor> CG2_to_DG1 = (Eigen::Matrix<double, 3, 9, Eigen::RowMajor>() << 0.02777777777777779, 0.11111111111111113, 0.02777777777777779, 0.11111111111111113, 0.4444444444444444, 0.11111111111111113, 0.02777777777777779, 0.11111111111111113, 0.02777777777777779, -0.16666666666666674, 2.6020852139652106e-18, 0.16666666666666674, -0.6666666666666667, -1.0408340855860843e-17, 0.6666666666666667, -0.16666666666666674, 0.0, 0.16666666666666674, -0.16666666666666674, -0.6666666666666667, -0.16666666666666674, 0.0, 0.0, 0.0, 0.16666666666666674, 0.6666666666666667, 0.16666666666666674).finished();

static const Eigen::Matrix<double, 6, 9, Eigen::RowMajor> CG2_to_DG2 = (Eigen::Matrix<double, 6, 9, Eigen::RowMajor>() << 0.02777777777777779, 0.11111111111111113, 0.02777777777777779, 0.11111111111111113, 0.4444444444444444, 0.11111111111111113, 0.02777777777777779, 0.11111111111111113, 0.02777777777777779, -0.16666666666666674, 2.6020852139652106e-18, 0.16666666666666674, -0.6666666666666667, -1.0408340855860843e-17, 0.6666666666666667, -0.16666666666666674, 0.0, 0.16666666666666674, -0.16666666666666674, -0.6666666666666667, -0.16666666666666674, 0.0, 0.0, 0.0, 0.16666666666666674, 0.6666666666666667, 0.16666666666666674, 0.3333333333333336, -0.6666666666666664, 0.3333333333333336, 1.3333333333333344, -2.6666666666666647, 1.3333333333333344, 0.3333333333333336, -0.6666666666666662, 0.3333333333333336, 0.3333333333333336, 1.3333333333333344, 0.3333333333333336, -0.6666666666666663, -2.6666666666666647, -0.6666666666666664, 0.3333333333333336, 1.3333333333333344, 0.3333333333333336, 1.0000000000000002, -3.122502256758253e-17, -1.0000000000000002, 0.0, 0.0, 0.0, -1.0000000000000002, 6.245004513516506e-17, 1.0000000000000004).finished();

//------------------------------------
#endif
