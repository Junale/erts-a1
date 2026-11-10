```mermaid
---
config:
  class:
    hideEmptyMembersBox: true
title: singleton GoF state  class diagram
---
classDiagram
    class EmbeddedSystemX{
        - state: SystemState
        + setState(SystemState*)
    }

    EmbeddedSystemX --> SystemState

    class SystemState{
        + invoke()
    }

    SystemState <|-- PowerOnSelfTest
    SystemState <|-- Failure
    SystemState <|-- Initializing
    SystemState <|-- Operational

    class PowerOnSelfTest{
        + invoke()
        + getInstance(): PowerOnSelfTest&
        - PowerOnSelfTest()
        - PowerOnSelfTest(const PowerOnSelfTest&)
        - operator=(const PowerOnSelfTest&): PowerOnSelfTest&
    }
    class Failure{
        + invoke()
        + getInstance(): Failure&
        - Failure()
        - Failure(const Failure&)
        - operator=(const Failure&): Failure&
    }
    class Initializing{
        + invoke()
        + getInstance(): Initializing&
        - Initializing()
        - Initializing(const Initializing&)
        - operator=(const Initializing&): Initializing&
    }
    class Operational{
        - state: OperationalState
        + invoke()
        + getInstance(): Operational&
        + setState(OperationalState*)
        - Operational()
        - Operational(const Operational&)
        - operator=(const Operational&): Operational&
    }

    Operational --> OperationalState


    class OperationalState {
        + invoke()
    }

    OperationalState <|-- Ready
    OperationalState <|-- Configuration
    OperationalState <|-- RealTimeLoop

    class Ready {
        + invoke()
        + getInstance(): Ready&
        - Ready()
        - Ready(const Ready&)
        - operator=(const Ready&): Ready&
    }

    class Configuration {
        + invoke()
        + getInstance(): Configuration&
        - Configuration()
        - Configuration(const Configuration&)
        - operator=(const Configuration&): Configuration&
    }
    class RealTimeLoop {
        - queue: ChmodeQueue*
        + invoke()
        + getInstance(): RealTimeLoop&
        + SetQueue(ChmodeQueue*)
        - RealTimeLoop()
        - RealTimeLoop(const RealTimeLoop&)
        - operator=(const RealTimeLoop&): RealTimeLoop&
    }


    RealTimeLoop --> ChmodeQueue : push
    RealTimeLoop --> ChmodeEvent

    class ChmodeEvent {
        + Type: enum
        + ChmodeEvent(Type)
        + getType(): Type
        - type: Type
    }

    ChmodeQueue --> ChmodeEvent

    class ChmodeQueue {
        - queue: std::queue~ChmodeEvent~
        + push(const ChmodeEvent&)
        + pop(): std::optional~ChmodeEvent~
    }

    ChmodeEventHandler --> ChmodeEvent
    ChmodeEventHandler --> ChmodeQueue : pop

    class ChmodeEventHandler {
        - queue: ChmodeQueue*
        + handle(ChmodeEvent)
        + runThread()
        + ChmodeEventHandler(ChmodeQueue*)
    }
```

## State pattern: system

```mermaid
---
config:
  class:
    hideEmptyMembersBox: true
title: system state pattern
---
classDiagram
    class EmbeddedSystemX {
        - state: SystemState*
        + setState(SystemState*)
    }

    class SystemState {
        + invoke()
    }

    EmbeddedSystemX --> SystemState
    SystemState <|-- PowerOnSelfTest
    SystemState <|-- Failure
    SystemState <|-- Initializing
    SystemState <|-- Operational

    class PowerOnSelfTest {
        + invoke()
        + getInstance(): PowerOnSelfTest&
        - PowerOnSelfTest()
        - PowerOnSelfTest(const PowerOnSelfTest&)
        - operator=(const PowerOnSelfTest&): PowerOnSelfTest&
    }

    class Failure {
        + invoke()
        + getInstance(): Failure&
        - Failure()
        - Failure(const Failure&)
        - operator=(const Failure&): Failure&
    }

    class Initializing {
        + invoke()
        + getInstance(): Initializing&
        - Initializing()
        - Initializing(const Initializing&)
        - operator=(const Initializing&): Initializing&
    }

    class Operational {
        + invoke()
        + getInstance(): Operational&
        - Operational()
        - Operational(const Operational&)
        - operator=(const Operational&): Operational&
    }
```

## State pattern: Operational substates

```mermaid
---
config:
  class:
    hideEmptyMembersBox: true
title: Operational state pattern
---
classDiagram
    class Operational {
        - state: OperationalState*
        + invoke()
        + getInstance(): Operational&
        + setState(OperationalState*)
        - Operational()
        - Operational(const Operational&)
        - operator=(const Operational&): Operational&
    }

    class OperationalState {
        + invoke()
    }

    Operational --> OperationalState
    OperationalState <|-- Ready
    OperationalState <|-- Configuration
    OperationalState <|-- RealTimeLoop

    class Ready {
        + invoke()
        + getInstance(): Ready&
        - Ready()
        - Ready(const Ready&)
        - operator=(const Ready&): Ready&
    }

    class Configuration {
        + invoke()
        + getInstance(): Configuration&
        - Configuration()
        - Configuration(const Configuration&)
        - operator=(const Configuration&): Configuration&
    }

    class RealTimeLoop {
        + invoke()
        + getInstance(): RealTimeLoop&
        - RealTimeLoop()
        - RealTimeLoop(const RealTimeLoop&)
        - operator=(const RealTimeLoop&): RealTimeLoop&
    }
```

## Active object pattern: RealTimeLoop

```mermaid
---
config:
  class:
    hideEmptyMembersBox: true
title: RealTimeLoop active object pattern
---
classDiagram
    class RealTimeLoop {
        - queue: ChmodeQueue*
        + invoke()
        + getInstance(): RealTimeLoop&
        + SetQueue(ChmodeQueue*)
        - RealTimeLoop()
        - RealTimeLoop(const RealTimeLoop&)
        - operator=(const RealTimeLoop&): RealTimeLoop&
    }

    class ChmodeEvent {
        + Type: enum
        + ChmodeEvent(Type)
        + getType(): Type
        - type: Type
    }

    class ChmodeQueue {
        - queue: std::queue~ChmodeEvent~
        + push(const ChmodeEvent&)
        + pop(): std::optional~ChmodeEvent~
    }

    class ChmodeEventHandler {
        - queue: ChmodeQueue*
        + handle(ChmodeEvent)
        + runThread()
        + ChmodeEventHandler(ChmodeQueue*)
    }

    RealTimeLoop --> ChmodeQueue : push
    RealTimeLoop --> ChmodeEvent : creates
    ChmodeQueue --> ChmodeEvent
    ChmodeEventHandler --> ChmodeQueue : pop
    ChmodeEventHandler --> ChmodeEvent : handles
```

## Libraries

The used c++ libraries:

- iostream: To output the states etc.
- threads: For running the handler asynchronously
