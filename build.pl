#!/bin/perl

my $DISPLAY = ":0";
my $PWD = `pwd`;
chomp $PWD;

if($ARGV[0] eq "build"){
	print `podman build -f Dockerfile.linux -t qtlin .`;
	print `podman build -f Dockerfile.linuxqt6 -t qtlin6 .`;
	print `podman build -f Dockerfile.win -t qtwin .`;
}if($ARGV[0] eq "buildwin"){
	print `podman build -f Dockerfile.win -t qtwin .`;
}if($ARGV[0] eq "buildlin"){
	print `podman build -f Dockerfile.linux -t qtlin .`;
}if($ARGV[0] eq "buildlin6"){
	print `podman build -f Dockerfile.linuxqt6 -t qtlin6 .`;
}elsif($ARGV[0] eq "lin"){
	print `podman run --volume $PWD:/CircuitTester:z -ti qtlin /bin/build`;
}elsif($ARGV[0] eq "lin6"){
	print `podman run --volume $PWD:/CircuitTester:z -ti qtlin6 /bin/build`;
}elsif($ARGV[0] eq "test"){
	print `podman run --volume $PWD:/CircuitTester:z -ti qtlin /bin/runtest`;
}elsif($ARGV[0] eq "win"){
	print `podman run --volume $PWD:/CircuitTester:z -ti qtwin /bin/build`;
}elsif($ARGV[0] eq "run"){
	print `podman run --volume $PWD:/CircuitTester:z -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -ti qtlin /CircuitTester/linbuild5/src/main/CircuitTester`;
}elsif($ARGV[0] eq "run6"){
	print `podman run --volume $PWD:/CircuitTester:z -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -ti qtlin6 /CircuitTester/linbuild6/src/main/CircuitTester`;
}else{
	print "Usage: build.sh [build|buildwin|buildlin|win|lin|test]";
}

print "\n";
