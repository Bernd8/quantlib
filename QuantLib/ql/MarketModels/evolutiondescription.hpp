/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Ferdinando Ametrano
 Copyright (C) 2006 Mark Joshi

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/


#ifndef quantlib_evolution_description_hpp
#define quantlib_evolution_description_hpp

#include <ql/Math/array.hpp>
#include <ql/Math/matrix.hpp>
#include <vector>

namespace QuantLib {

    /*! This class stores:
		1) evolutionTimes = the times defining the rates that are to be evolved,
		2) rateTimes = the times at which the rates need to be known, 
		3) numeraires = the numeraire to be used for each step. 
		4) relevanceRates = which rates need to be known at each time.
        This class is really just a 3-tuple of the evolutions times, the rate
        times and the numeraires.
        
        - There will be n+1 rate times expressing payment and reset times of
        forward rates.
		- There will be any number of evolution times.
		- There will be one numeraire index for each evolution time. The
        numeraire is an index amongst the rate times so it ranges from 0 to n.
        The numeraire must not have expired before the end of the step.
        Note that in the terminal measure, one would take the index of the
        last bond as numeraire. Whereas the discretely compounding money market
        account would take the index of the first unexpired bond for each step.
        Default values for numeraires will be the final bond.
		- We also store which part of the rates are relevant for pricing via
        relevance rates. The important part for the i-th step will then range
        from relevanceRates[i].first to relevanceRates[i].second
        Default values for relevance rates will be 0 and n. 

        - Example n = 5:
           |-----|-----|-----|-----|-----|      (size = 6)
           t0    t1    t2    t3    t4    t5     rateTimes
           f0    f1    f2    f3    f4           forwardRates
           d0    d1    d2    d3    d4    d5     discountBonds
           d0/d0 d1/d0 d2/d0 d3/d0 d4/d0 d5/d0  discountRatios
           sr0   sr1   sr2   sr3   sr4          coterminalSwaps
        */

    class EvolutionDescription {
        // This typedef is used so that gcc 3.3 can parse the
        // constructor declaration (not even parenthesizing the
        // default argument worked)
        typedef std::pair<Size,Size> range;
      public:
        EvolutionDescription();
        EvolutionDescription(
            const std::vector<Time>& rateTimes,
            const std::vector<Time>& evolutionTimes,
            const std::vector<std::pair<Size,Size> >& relevanceRates =
                                                        std::vector<range>());
        const std::vector<Time>& rateTimes() const;
        const std::vector<Time>& rateTaus() const;
        const std::vector<Time>& evolutionTimes() const;
        const Matrix& effectiveStopTime() const;
        const std::vector<Size>& firstAliveRate() const;
        const std::vector<std::pair<Size,Size> >& relevanceRates() const;
        Size numberOfRates() const;     // returns rateTimes().size() - 1
        Size numberOfSteps() const;     // returns evolutionTimes().size()

        //const std::vector<Size>& numeraires() const;
        //void setNumeraires(const std::vector<Size>&);

        //void setTerminalMeasure();
        //bool isInTerminalMeasure() const;

        //void setMoneyMarketMeasure();
        //bool isInMoneyMarketMeasure() const;

        //void setMoneyMarketPlusMeasure(Size offset = 1);
        //bool isInMoneyMarketPlusMeasure(Size offset = 1) const;

      private:
        std::vector<Time> rateTimes_, evolutionTimes_;
        Size steps_;
        //std::vector<Size> numeraires_;
        std::vector<std::pair<Size,Size> > relevanceRates_;
        std::vector<Time> rateTaus_;
        Matrix effStopTime_;
        std::vector<Size> firstAliveRate_;
    };


    // Numeraire functions

    void checkCompatibility(const EvolutionDescription& evolution,
                            const std::vector<Size>& numeraires);
    bool isInTerminalMeasure(const EvolutionDescription& evolution,
                             const std::vector<Size>& numeraires);
    bool isInMoneyMarketPlusMeasure(const EvolutionDescription& evolution,
                                    const std::vector<Size>& numeraires,
                                    Size offset = 1);
    bool isInMoneyMarketMeasure(const EvolutionDescription& evolution,
                                const std::vector<Size>& numeraires);

    std::vector<Size> terminalMeasure(const EvolutionDescription& evolution);
    std::vector<Size> moneyMarketPlusMeasure(const EvolutionDescription&,
                                             Size offset = 1);
    std::vector<Size> moneyMarketMeasure(const EvolutionDescription&);

}

#endif
