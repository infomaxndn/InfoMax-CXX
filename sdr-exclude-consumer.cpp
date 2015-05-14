/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2014 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * @author Ilya Moiseenko iliamo@ucla.edu
 */

// correct way to include ndn-cxx headers
// #include <ndn-cxx/contexts/consumer-context.hpp>
#include "contexts/consumer-context.hpp"

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace examples {

class CallbackContainer
{
public:
  CallbackContainer()
  {
  }
  
  void
  processPayload(const uint8_t* buffer, size_t bufferSize)
  {
    std::string content1((char*)buffer);
    
    std::cout << "REASSEMBLED " << content1 << std::endl;
  }
  
  void
  processData(Data& data)
  {
    std::cout << "DATA IN CNTX" << std::endl;
  }
  
  void
  processLeavingInterest(Interest& interest)
  {
    std::cout << "LEAVES " << interest.toUri() << std::endl;
  }  
};

int
main(int argc, char** argv)
{
  Name sampleName("/a/b/c");
      
  CallbackContainer stubs;

  Consumer c(sampleName, SDR);
  
  Exclude exclusion;
  exclusion.excludeOne(Name("k").get(0));
   
  c.setContextOption(EXCLUDE_S, exclusion); // /a/b/c/k will be excluded
  c.setContextOption(MUST_BE_FRESH_S, true);
    
  c.setContextOption(INTEREST_LEAVE_CNTX, 
                    (InterestCallback)bind(&CallbackContainer::processLeavingInterest, &stubs, _1));
  
  c.setContextOption(DATA_ENTER_CNTX, 
                    (DataCallback)bind(&CallbackContainer::processData, &stubs, _1));
  
  c.setContextOption(CONTENT_RETRIEVED, 
                    (ContentCallback)bind(&CallbackContainer::processPayload, &stubs, _1, _2));
  
  c.consume(Name());
  
  return 0;
}

} // namespace examples
} // namespace ndn

int
main(int argc, char** argv)
{
  return ndn::examples::main(argc, argv);
}
