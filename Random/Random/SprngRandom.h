/*----------------------
 GATE version name: gate_v6

 Copyright (C): OpenGATE Collaboration

 This software is distributed under the terms
 of the GNU Lesser General  Public Licence (LGPL)
 See GATE/LICENSE.txt for further details
 ----------------------*/

/*
 \class  SprngRandom.h
 \author copernicus email:copernicus231@gmail.com
 */

#ifndef SPRNGRANDOM_H_
#define SPRNGRANDOM_H_ 1
#include "CLHEP/Random/defs.h"
#include "CLHEP/Random/RandomEngine.h"
class Sprng;
namespace CLHEP {

class SprngRandom: public HepRandomEngine {
public:
	enum random_engine{
		LFG,
		LCG,
		LCG64,
		CMRG,
		MLFG,
		PMLCG

	};
	SprngRandom();
	SprngRandom(random_engine);
	SprngRandom(random_engine,int n_streams,int stream_id);
	SprngRandom(long seed);
	virtual ~SprngRandom();
	double flat();
	// Returns a pseudo random number between 0 and 1
	// (excluding the end points)

	void flatArray(const int size, double* vect);
	// Fills the array "vect" of specified size with flat random values.

	void setSeed(long seed, int dum = 0);
	// Sets the state of the algorithm according to seed.

	void setSeeds(const long * seeds, int dum = 0);
	// Sets the state of the algorithm according to the zero terminated
	// array of seeds. Only the first seed is used.

	void saveStatus(const char filename[] = "SprngRand.conf") const;
	// Saves on file JamesRand.conf the current engine status.

	void restoreStatus(const char filename[] = "SprngRand.conf");
	// Reads from file JamesRand.conf the last saved engine status
	// and restores it.

	void showStatus() const;
	// Dumps the engine status on the screen.


	virtual std::ostream & put(std::ostream & os) const;
	virtual std::istream & get(std::istream & is);
	static std::string beginTag();
	virtual std::istream & getState(std::istream & is);

	std::string name() const;
	static std::string engineName() {
		return "SprngRandom";
	}

	std::vector<unsigned long> put() const;
	bool get(const std::vector<unsigned long> & v);
	bool getState(const std::vector<unsigned long> & v);

	//static const unsigned int VECTOR_STATE_SIZE = 202;

private:
	Sprng *engine;
	int n_streams;
	int stream_id;
};

} /* namespace CLHEP */
#ifdef ENABLE_BACKWARDS_COMPATIBILITY
//  backwards compatibility will be enabled ONLY in CLHEP 1.9
using namespace CLHEP;
#endif
#endif /* SPRNGRANDOM_H_ */
