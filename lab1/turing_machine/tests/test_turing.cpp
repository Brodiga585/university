#include <iostream>
#include <cassert>
#include <fstream>
#include "Tape.hpp"
#include "Head.hpp"
#include "Rule.hpp"
#include "TuringMachine.hpp"

void testTape() {
    Tape tape('_');
    tape.write(0, '1');
    tape.write(1, '0');
    tape.write(2, 'X');
    tape.write(2, '_');

    assert(tape[0] == '1');
    assert(tape[1] == '0');
    assert(tape[5] == '_');
    
    tape.loadFromString("10");
    assert(tape.toString(0) == "| [1] 0  |");
    assert(tape.toString(5).find("[_]") != std::string::npos);

    Tape tape2('_');
    tape2.loadFromString("10");
    assert(tape == tape2);
    assert(!(tape != tape2));
    
    std::cout << "[PASSED] testTape\n";
}

void testHead() {
    Head head(0);
    assert(head.getPosition() == 0);

    head.setPosition(5);
    assert(head.getPosition() == 5);

    ++head;
    assert(head.getPosition() == 6);

    --head;
    assert(head.getPosition() == 5);

    Head head2(5);
    assert(head == head2);
    assert(!(head != head2));

    head2++;
    assert(head2.getPosition() == 6);

    head2--;
    assert(head2.getPosition() == 5);

    std::cout << "[PASSED] testHead\n";
}

void testRule() {
    Rule rule("q0", '1', "q0", '0', 'R');
    assert(rule.getCurrentState() == "q0");
    assert(rule.getReadSymbol() == '1');
    assert(rule.getNextState() == "q0");
    assert(rule.getWriteSymbol() == '0');
    assert(rule.getDirection() == 'R');

    Rule rule2("q0", '1', "q0", '0', 'R');
    assert(rule == rule2);
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

    assert(tm.getCurrentState() == "q0");
    assert(tm.getHead().getPosition() == 0);
    assert(tm.getTape()[0] == '1');

    int steps = tm.run(true);
    assert(steps == 3);
    assert(tm.getTape()[0] == '0');
    assert(tm.getTape()[1] == '0');

    TuringMachine tm2;
    tm2.setHaltState("halt");
    assert(tm2.step() == false);

    std::cout << "[PASSED] testTuringMachineExecution\n";
}

void testMachineLoadFileAndExceptions() {
    TuringMachine tm;
    try {
        tm.loadFromFile("nonexistent_file.txt");
        assert(false);
    } catch (const std::runtime_error& e) {
    }

    std::ofstream out("test_machine.txt");
    out << "# Comment line\n";
    out << "state: q1\n";
    out << "tape: AB\n";
    out << "head: -1\n";
    out << "q1 A q2 B L\n";
    out.close();

    tm.loadFromFile("test_machine.txt");
    assert(tm.getCurrentState() == "q1");
    assert(tm.getTape()[-1] == '_');
    assert(tm.getTape()[0] == 'A');
    assert(tm.getTape()[1] == 'B');
    assert(tm.getHead().getPosition() == -1);

    std::cout << "[PASSED] testMachineLoadFileAndExceptions\n";
}

int main() {
    std::cout << "=== RUNNING UNIT TESTS ===\n";
    try {
        testTape();
        testHead();
        testRule();
        testTuringMachineExecution();
        testMachineLoadFileAndExceptions();
        std::cout << "=== ALL TESTS PASSED SUCCESSFULLY ===\n";
    } catch (const std::exception& e) {
        std::cerr << "[FAILED] Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}