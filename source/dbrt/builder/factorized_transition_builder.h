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
 * \file factorized_transition_builder.h
 * \date January 2016
 * \author Jan Issac (jan.issac@gmail.com)
 */

#pragma once

#include <Eigen/Dense>
#include <dbot/builder/transition_function_builder.h>
#include <dbrt/builder/exceptions.h>
#include <fl/model/transition/linear_transition.hpp>
#include <fl/util/meta.hpp>
#include <fl/util/profiling.hpp>
#include <memory>

namespace dbrt
{
template <typename Tracker>
class FactorizedTransitionBuilder
{
public:
    enum Dimension
    {
        StateDim = Tracker::JointStateDim,
        NoiseDim = Tracker::JointNoiseDim,
        InputDim = Tracker::JointInputDim
    };

    typedef typename Tracker::JointState State;
    typedef typename Tracker::JointNoise Noise;
    typedef typename Tracker::JointInput Input;
    typedef typename Tracker::JointStateModel Model;

    struct Parameters
    {
        // double joint_sigma;
        std::vector<double> joint_sigmas;
        std::vector<double> bias_sigmas;
        std::vector<double> bias_factors;
        int joint_count;
    };

    FactorizedTransitionBuilder(const Parameters& param) : param_(param) {}
    virtual std::shared_ptr<Model> build(int joint_index) const
    {
        if (param_.joint_count != param_.joint_sigmas.size())
        {
            throw InvalidNumberOfJointSigmasException();
        }
        if (param_.joint_count != param_.bias_sigmas.size())
        {
            throw InvalidNumberOfJointSigmasException();
        }
        if (param_.joint_count != param_.bias_factors.size())
        {
            throw InvalidNumberOfJointSigmasException();
        }

        if (joint_index < 0 || joint_index >= param_.joint_count)
        {
            throw JointIndexOutOfBoundsException();
        }

        if (StateDim != 2 || NoiseDim != 2 || InputDim != 1)
        {
            std::cout << "damn you screwed up dimensions" << std::endl;
            exit(-1);
        }

        auto model = std::make_shared<Model>(StateDim, NoiseDim, InputDim);

        auto A = model->create_dynamics_matrix();
        auto B = model->create_noise_matrix();
        auto C = model->create_input_matrix();

        A.setIdentity();
        A(1, 1) = param_.bias_factors[joint_index];

        B.setIdentity();
        B(0, 0) = param_.joint_sigmas[joint_index];
        B(1, 1) = param_.bias_sigmas[joint_index];

        model->dynamics_matrix(A);
        model->noise_matrix(B);
        model->input_matrix(C);

        return model;
    }

private:
    Parameters param_;
};
}
