// BaseComponent.h
#pragma once

#include "Mediator/Mediator.h"

class BaseComponent {
public:
    BaseComponent();
    void setMediator(Mediator* mediator);

protected:
    Mediator* _mediator;
};