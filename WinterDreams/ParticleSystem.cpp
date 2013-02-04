#include "ParticleSystem.h"
#include <cassert>
#include <cstdlib>

ParticleSystem::ParticleSystem(){};

void ParticleSystem::newParticle(std::string name, std::string filePath, int lifetime){
	ParticleType ptype;
	ptype.sLifetime=lifetime;
	ptype.sTexture=filePath;
	mParticleTypesMap.insert(std::pair<std::string, ParticleType>(name, ptype));
	std::deque<Particle> deque;
	mParticleDequesMap.insert(std::pair<std::string, std::deque<Particle> >(name, deque));
}

void ParticleSystem::deleteParticle(std::string type){
	mParticleTypesMap.erase(type);
}

void ParticleSystem::createParticle(std::string type, sf::Vector2f position, sf::Vector2f speed){
	std::map<std::string, ParticleType>::iterator it;
    it=mParticleTypesMap.find(type);
    if (it!=mParticleTypesMap.end()){
		Particle part(position, speed, it->second.sLifetime, it->second.sTexture);
		mParticleDequesMap.find(type)->second.push_back(part);
    } else {
        assert(false);
    }
}

void ParticleSystem::updateSystem(){
	std::map<std::string, std::deque<Particle> >::iterator it;
	it = mParticleDequesMap.begin();
	for(;it!=mParticleDequesMap.end();++it){
		for (size_t j=0; j<it->second.size();++j){
			it->second[j].update();
			it->second[j].drawSelf();
			if (it->second[j].getDead()==0){
				it->second.erase(it->second.begin()+j);
				--j;
			}
		}
	}
}

void ParticleSystem::setParticleSpeed(sf::Vector2f speed){
	std::map<std::string, std::deque<Particle> >::iterator it;
	it = mParticleDequesMap.begin();
	for(size_t i=0; i<mParticleTypesMap.size();++i){
		for (size_t j=0; j<it->second.size();++j){
			it->second[j].setSpeed(speed);
		}
	}
}

void ParticleSystem::adjustParticleSpeed(sf::Vector2f speedAdjust, sf::Vector2f modSpeedAdjust){
	std::map<std::string, std::deque<Particle> >::iterator it;
	it = mParticleDequesMap.begin();
	for(size_t i=0; i<mParticleTypesMap.size();++i){
		for (size_t j=0; j<it->second.size();++j){
			float rVal = static_cast<float>(rand()) / RAND_MAX;
			auto modVector = sf::Vector2f(modSpeedAdjust.x*rVal,modSpeedAdjust.y*rVal);
			it->second[j].adjustSpeed(speedAdjust+modVector);
		}
	}
}

void ParticleSystem::addEmitter(int x, int y, int w, int h){
}

void ParticleSystem::removeEmitter(){
}

unsigned int ParticleSystem::countParticles(std::string type){
	return 0;
}

unsigned int ParticleSystem::countTotalParticles(){
	unsigned int total=0;
	std::map<std::string, std::deque<Particle> >::iterator it;
	it = mParticleDequesMap.begin();
	for(size_t i=0; i<mParticleTypesMap.size();++i){
		total+=it->second.size();
	}
	return total;
}