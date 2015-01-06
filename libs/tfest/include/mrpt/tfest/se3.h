/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2015, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */
#pragma once

#include <mrpt/utils/utils_defs.h>
#include <mrpt/math/math_frwds.h>
#include <mrpt/math/CMatrixFixedNumeric.h>
#include <mrpt/utils/TMatchingPair.h>
#include <mrpt/tfest/link_pragmas.h>
#include <mrpt/poses/poses_frwds.h>

namespace mrpt
{
	namespace tfest
	{
		/** \addtogroup mrpt_tfest_grp
		  * @{ */

		/** Least-squares (L2 norm) solution to finding the optimal SE(3) transform between two reference frames using the "quaternion" or Horn's method: 
		  *  - "Closed-form solution of absolute orientation using unit quaternions", BKP Horn, Journal of the Optical Society of America, 1987.
		  *
		  *  The optimal transformation `q` fulfills \f$ p_{this} = q \oplus p_{other} \f$, that is, the 
		  *  transformation of frame `other` with respect to `this`.
		  *
		  *  \image html tfest_frames.png
		  *
		  * \param[in]  in_correspondences  The coordinates of the input points for the two coordinate systems "this" and "other"
		  * \param[out] out_transform       The output transformation
		  * \param[out] out_scale           The computed scale of the optimal transformation (will be 1.0 for a perfectly rigid translation + rotation).
		  * \param[in]  forceScaleToUnity   Whether or not force the scale employed to rotate the coordinate systems to one (rigid transformation)
		  * \note [New in MRPT 1.3.0] This function replaces mrpt::scanmatching::leastSquareErrorRigidTransformation6DRANSAC() and mrpt::scanmatching::HornMethod()
		  * \sa se2_l2, se3_l2_robust
		  */
		bool TFEST_IMPEXP se3_l2(
			const mrpt::utils::TMatchingPairList  & in_correspondences,
			mrpt::poses::CPose3DQuat   & out_transform,
			double                     & out_scale,
			bool                         forceScaleToUnity = false );

		/** \overload 
		  *
		  * This version accepts corresponding points as two vectors of TPoint3D (must have identical length).
		  */
		bool TFEST_IMPEXP se3_l2(
			const std::vector<mrpt::math::TPoint3D> & in_points_this,
			const std::vector<mrpt::math::TPoint3D> & in_points_other,
			mrpt::poses::CPose3DQuat   & out_transform,
			double                     & out_scale,
			bool                         forceScaleToUnity = false );

		/** Parameters for se3_l2_robust(). See function for more details */
		struct TFEST_IMPEXP TSE3RobustParams
		{
			unsigned int  ransac_minSetSize; //!< (Default=5)  The minimum amount of points in a set to be accepted
			unsigned int  ransac_nmaxSimulations; //!< (Default=50) The maximum number of iterations of the RANSAC algorithm
			double        ransac_maxSetSizePct; //!< (Default=0.7) The (minimum) assumed percent (0.0 - 1.0) of the input set to be considered as inliers
			bool          forceScaleToUnity; //!< (Default=true) 

			TSE3RobustParams() :
				ransac_minSetSize( 5 ),
				ransac_nmaxSimulations(50),
				ransac_maxSetSizePct(0.7),
				forceScaleToUnity( true)
			{
			}
		};

		/** Output placeholder for se3_l2_robust() */
		struct TFEST_IMPEXP TSE3RobustResult
		{
			mrpt::math::TPose3DQuat         transformation; //!< The best transformation found
			double                          scale;          //!< The estimated scale of the rigid transformation (should be very close to 1.0)
			mrpt::vector_int                inliers_idx;    //!< Indexes within the `in_correspondences` list which corresponds with inliers

			TSE3RobustResult() : scale(.0)  { }
		};


		/** Least-squares (L2 norm) solution to finding the optimal SE(3) transform between two reference frames using RANSAC and the "quaternion" or Horn's method: 
		  *  - "Closed-form solution of absolute orientation using unit quaternions", BKP Horn, Journal of the Optical Society of America, 1987.
		  *
		  *  The optimal transformation `q` fulfills \f$ p_{this} = q \oplus p_{other} \f$, that is, the 
		  *  transformation of frame `other` with respect to `this`.
		  *
		  *  \image html tfest_frames.png
		  *
		  * \param[in] in_correspondences The set of correspondences.
		  * \param[in] in_params Method parameters (see docs for TSE3RobustParams)
		  * \param[out] out_results Results: transformation, scale, etc.
		  *
		  * \return True if the minimum number of correspondences was found, false otherwise.
		  * \note Implemented by FAMD, 2008. Re-factored by JLBC, 2015.
		  * \note [New in MRPT 1.3.0] This function replaces mrpt::scanmatching::leastSquareErrorRigidTransformation6DRANSAC()
		  * \sa se2_l2, se3_l2
		  */
		bool TFEST_IMPEXP se3_l2_robust(
			const mrpt::utils::TMatchingPairList & in_correspondences,
			const TSE3RobustParams               & in_params,
			TSE3RobustResult                     & out_results );


		/** @} */  // end of grouping

	}

} // End of namespace
