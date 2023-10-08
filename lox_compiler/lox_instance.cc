#include "lox_instance.h"
#include "error.h"
#include <utility>

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass): klass{std::move(klass)} {}

std::any LoxInstance::get(const Token& name) {
	auto elem = fields.find(name.lexeme);
	if (elem != fields.end()) {
		return elem->second;
	}

	std::shared_ptr<LoxFunction> method = klass->findMethod(name.lexeme);
}