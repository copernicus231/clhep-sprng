/*----------------------
 GATE version name: gate_v6

 Copyright (C): OpenGATE Collaboration

 This software is distributed under the terms
 of the GNU Lesser General  Public Licence (LGPL)
 See GATE/LICENSE.txt for further details
 ----------------------*/

/*
 \class  SprngRandom.cc
 \author copernicus email:copernicus231@gmail.com
 */

#include "CLHEP/Random/SprngRandom.h"
#include "CLHEP/Random/engineIDulong.h"


#include <cmath>
#include <cstdlib>
#include "sprng_cpp.h"

namespace CLHEP {

std::string SprngRandom::name() const {
	return "SprngRandom";
}

SprngRandom::SprngRandom() :
		HepRandomEngine() {
	engine=SelectType(0);
	this->n_streams=1;
	this->stream_id=0;
	engine->init_rng(0,1,0,SPRNG_DEFAULT);
}

SprngRandom::SprngRandom(random_engine engine_type):HepRandomEngine(){
	engine=SelectType(engine_type);
	this->n_streams=1;
	this->stream_id=0;
	engine->init_rng(0,1,0,SPRNG_DEFAULT);
}

SprngRandom::SprngRandom(random_engine engine_type,int n_streams,int stream_id):HepRandomEngine(){
	engine=SelectType(engine_type);
	this->n_streams=n_streams;
	this->stream_id=stream_id;
	engine->init_rng(stream_id,n_streams,0,SPRNG_DEFAULT);
}

SprngRandom::~SprngRandom() {
	delete engine;
}

SprngRandom::SprngRandom(long seed):HepRandomEngine() {
	engine=SelectType(0);
	this->n_streams=1;
	this->stream_id=0;
	engine->init_rng(0,1,seed,SPRNG_DEFAULT);
}

double SprngRandom::flat() {
	return engine->get_rn_dbl();
}

void SprngRandom::flatArray(const int size, double *vect) {
	int i;
	for (i=0; i<size; ++i) {
		vect[i] = flat();
	}
}

void SprngRandom::setSeed(long seed, int dum) {
	engine->init_rng(stream_id,n_streams,seed,SPRNG_DEFAULT);
}

void SprngRandom::setSeeds(const long *seeds, int dum) {
	engine->init_rng(stream_id,n_streams,seeds[0],SPRNG_DEFAULT);//TODO FIX IT
}

void SprngRandom::saveStatus(const char filename[]) const {

	char *bytes;
	int size = 0;
	std::ofstream outFile( filename, std::ios::out|std::ios::binary) ;

	if(!outFile.is_open()){
		std::cerr << "  -- Error Saving SprngRandom status\n";
		return;
	}
	size = engine->pack_sprng(&bytes);	// pack stream state into an array
	outFile.write((char*)&size,sizeof(int));
	outFile.write(bytes,size);
	outFile.close();
}

void SprngRandom::restoreStatus(const char filename[]) {
  std::ifstream inFile( filename, std::ios::in|std::ios::binary);
  int size = 0;
  char buffer[MAX_PACKED_LENGTH];

  if(!checkFile(inFile,filename,engineName(),"restoreStatus")){
	 std::cerr << "  -- Engine state remains unchanged\n";
	 return;
  }

  inFile.read((char*)&size,sizeof(int));
  inFile.read(buffer,size);

  if(engine->unpack_sprng(buffer) == 0){
	 std::cerr << "  -- Engine stored doesn't match current Engine\n";
	 inFile.close();
	 return;
  }
  inFile.close();
}

void SprngRandom::showStatus() const {
	   std::cout << std::endl;
	   std::cout << "----- Sprng Random engine status -----" << std::endl;
	   std::cout << " Initial seed = "<<engine->get_rn_dbl();
	   engine->print_rng();

	   std::cout << "----------------------------------------" << std::endl;
}


std::ostream & SprngRandom::put(std::ostream & os) const {
	return os;//TODO
}

std::istream & SprngRandom::get(std::istream & is) {
	return is; //TODO
}

std::string SprngRandom::beginTag() {
	  return "SprngRandom-begin";
}

std::istream & SprngRandom::getState(std::istream & is) {
	return is;//TODO
}

std::vector<unsigned long> SprngRandom::put() const {
	std::vector<unsigned long> a;
	return a;//TODO
}

bool SprngRandom::get(const std::vector<unsigned long> & v) {
	return true;//TODO
}

bool SprngRandom::getState(const std::vector<unsigned long> & v) {
	return true;//TODO
}

} /* namespace CLHEP */
