module main.main;

import std.stdio;
import std.datetime;

import core.thread;

int main(string[] args){

	int div = 10000;

	writeln(Clock.currTime().stdTime/div);
	Thread.sleep(dur!("seconds")(1));
	writeln(Clock.currTime().stdTime/div);

	return 0;
}
