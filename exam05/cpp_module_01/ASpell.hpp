#ifndef ASPELL_HPP
# define ASPELL_HPP

#include <iostream>
#include "ATarget.hpp"
class ATarget;

class ASpell
{
protected:
	std::string name;
	std::string effects;

public:
	ASpell();
	virtual ~ASpell();
	ASpell(std::string const &n, std::string const &e);
	ASpell(ASpell const &);
	ASpell &operator=(ASpell const &);

	std::string const &getName() const;
	std::string const &getEffects() const;

	virtual ASpell* clone() const = 0;

	void launch(ATarget const &) const;
};

#endif
