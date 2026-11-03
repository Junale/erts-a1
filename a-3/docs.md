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
        + getInstance(): PowerOnSelfTest
    }
    class Failure{
        + invoke()
        + getInstance(): Failure
    }
    class Initializing{
        + invoke()
        + getInstance(): Initializing
    }
    class Operational{
        - state: OperationalState
        + invoke()
        + getInstance(): Operational
        + setState(OperationalState*)
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
        + getInstance(): Ready
    }

    class Configuration {
        + invoke()
        + getInstance(): Configuration
    }
    class RealTimeLoop {
        - queue: ChmodeQueue*
        + invoke()
        + getInstance(): RealTimeLoop
        + setQueue(ChmodeQueue*)
    }


    RealTimeLoop --> ChmodeQueue : push
    RealTimeLoop --> ChmodeEvent

    class ChmodeEvent {

    }

    ChmodeQueue --> ChmodeEvent

    class ChmodeQueue {
        - queue: ChmodeEvent[]
        + push(ChmodeEvent)
        + pop(): ChmodeEvent
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

## Libraries

The used c++ libraries:

- iostream: To output the states etc.
- threads: For running the handler asynchronously
