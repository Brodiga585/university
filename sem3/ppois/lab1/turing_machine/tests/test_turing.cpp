#include <iostream>
#include <cassert>
#include "Tape.hpp"
#include "Head.hpp"
#include "Rule.hpp"
#include "TuringMachine.hpp"

void testTape() {
    Tape tape('_');
    tape.write(0, '1');
    tape.write(1, '0');

    assert(tape[0] == '1');
    assert(tape[1] == '0');
    assert(tape[5] == '_');
    std::cout << "[PASSED] testTape\n";
}

void testHead() {
    Head head(0);
    assert(head.getPosition() == 0);

    ++head;
    assert(head.getPosition() == 1);

    --head;
    assert(head.getPosition() == 0);
    std::cout << "[PASSED] testHead\n";
}

void testRule() {
    Rule rule("q0", '1', "q0", '0', 'R');
    assert(rule.getCurrentState() == "q0");
    assert(rule.getReadSymbol() == '1');
    assert(rule.getNextState() == "q0");
    assert(rule.getWriteSymbol() == '0');
    assert(rule.getDirection() == 'R');
    std::cout << "[PASSED] testRule\n";
}

void testTuringMachineExecution() {
    TuringMachine tm;
    Tape tape('_');
    tape.loadFromString("11");
    tm.setTape(tape);
    tm.setHead(Head(0));
    tm.setCurrentState("q0");
    tm.setHaltState("halt");

    tm.addRule(Rule("q0", '1', "q0", '0', 'R'));
    tm.addRule(Rule("q0", '_', "halt", '_', 'S'));

    int steps = tm.run(false);
    assert(steps == 3);
    assert(tm.getTape()[0] == '0');
    assert(tm.getTape()[1] == '0');
    std::cout << "[PASSED] testTuringMachineExecution\n";
}

int main() {
    std::cout << "=== RUNNING UNIT TESTS ===\n";
    try {
        testTape();
        testHead();
        testRule();
        testTuringMachineExecution();
        std::cout << "=== ALL TESTS PASSED SUCCESSFULLY ===\n";
    } catch (const std::exception& e) {
        std::cerr << "[FAILED] Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}