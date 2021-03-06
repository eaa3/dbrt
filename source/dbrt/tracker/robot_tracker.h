/*
 * This is part of the Bayesian Object Tracking (bot),
 * (https://github.com/bayesian-object-tracking)
 *
 * Copyright (c) 2015 Max Planck Society,
 * 				 Autonomous Motion Department,
 * 			     Institute for Intelligent Systems
 *
 * This Source Code Form is subject to the terms of the GNU General Public
 * License License (GNU GPL). A copy of the license can be found in the LICENSE
 * file distributed with this source code.
 */

/**
 * \file robot_tracker.h
 * \date December 2015
 * \author Jan Issac (jan.issac@gmail.com)
 * \author Manuel Wuthrich (manuel.wuthrich@gmail.com)
 */

#pragma once

#include <Eigen/Dense>
#include <dbrt/robot_state.h>
#include <fl/util/types.hpp>
#include <mutex>
#include <vector>

// #include <dbot/pose/composed_vector.h>
// #include <dbot/pose/free_floating_rigid_bodies_state.h>
// #include <dbot/pose/pose_vector.h>
// #include <dbrt/kinematics_from_urdf.h>
// #include <image_transport/image_transport.h>
// #include <memory>
// #include <ros/ros.h>
// #include <string>

namespace dbrt
{
/**
 * \brief Abstract RobotTracker context
 */
class RobotTracker
{
public:
    typedef RobotState<> State;
    typedef Eigen::Matrix<fl::Real, Eigen::Dynamic, 1> Obsrv;
    typedef Eigen::Matrix<fl::Real, Eigen::Dynamic, 1> Noise;
    typedef Eigen::Matrix<fl::Real, Eigen::Dynamic, 1> Input;

public:
    /**
     * \brief Creates the tracker
     *
     * \param filter
     *     Rbc particle filter instance
     */
    RobotTracker() {}
    /**
     * \brief Overridable default destructor
     */
    virtual ~RobotTracker() {}
    /**
     * \brief perform a single filter step
     *
     * \param image
     *     Current observation image
     */
    virtual State track(const Obsrv& obsrv) = 0;

    /**
     * \brief Initializes the particle filter with the given initial states and
     *     the number of evaluations
     * @param initial_states
     * @param evaluation_count
     */
    virtual void initialize(const std::vector<State>& initial_states) = 0;

    /**
     * \brief Shorthand for a zero input vector
     */
    Input zero_input() const;

protected:
    mutable std::mutex mutex_;
};
}
