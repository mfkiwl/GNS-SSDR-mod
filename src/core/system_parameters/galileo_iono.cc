/*!
 * \file gps_iono.cc
 * \brief  Interface of a GPS IONOSPHERIC MODEL storage
 *
 * See http://www.gps.gov/technical/icwg/IS-GPS-200E.pdf Appendix II
 * \author Javier Arribas, 2013. jarribas(at)cttc.es
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2013  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#include "galileo_iono.h"

Galileo_Iono::Galileo_Iono()
{
    //valid = false;
	/*Ionospheric correction*/
	/*Az*/
	 ai0_5 = 0;	//Effective Ionisation Level 1st order parameter [sfu]
	 ai1_5 = 0;		//Effective Ionisation Level 2st order parameter [sfu/degree]
	 ai2_5 = 0;		//Effective Ionisation Level 3st order parameter [sfu/degree]

	/*Ionospheric disturbance flag*/
	 Region1_flag_5 = false;	// Ionospheric Disturbance Flag for region 1
	 Region2_flag_5 = false;	// Ionospheric Disturbance Flag for region 2
	 Region3_flag_5 = false;	// Ionospheric Disturbance Flag for region 3
	 Region4_flag_5 = false;	// Ionospheric Disturbance Flag for region 4
	 Region5_flag_5 = false;	// Ionospheric Disturbance Flag for region 5

	 t0t_6 = 0;
     WNot_6 = 0;

}

