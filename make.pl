#!/bin/perl
use Cwd;

use warnings;
use strict;

my $ROOT = getcwd;

sub make_win {
	`rm -rf winbuild5`;
	`mkdir -p winbuild5`;
	
	`podman run -v $ROOT:/CircuitTester:z -it --rm qtwin build`
}

sub make_lin {
	`rm -rf linbuild5`;
	`mkdir -p linbuild5`;
	
	`podman run -v $ROOT:/CircuitTester:z -it --rm qtlin build`
}

sub move {
	`mkdir -p out`;
	`cp -r $ROOT/linbuild5/src/main $ROOT/out/linux`;
	`cp -r $ROOT/winbuild5/src/main $ROOT/out/windows`;
	`rm -rf $ROOT/out/windows/CMakeFiles $ROOT/out/windows/*_autogen $ROOT/out/windows/Makefile $ROOT/out/windows/*.cmake`;
	`rm -rf $ROOT/out/linux/CMakeFiles $ROOT/out/linux/*_autogen $ROOT/out/linux/Makefile $ROOT/out/linux/*.cmake`;
	
	chdir "$ROOT/out/linux";
	`zip -rq $ROOT/out/linux.zip .`;
	
	chdir "$ROOT/out/windows";
	`zip -rq $ROOT/out/windows.zip .`;
	
	chdir "$ROOT/";
	
	`rm -rf $ROOT/out/windows $ROOT/out/linux`;
}

sub archive {

}

`rm -rf out/`;

make_win();
make_lin();
move();
archive();
