#!/bin/perl

if($ARGV[0] eq "build"){
	print `podman build -f Dockerfile.linux -t qtlin .`;
	print `podman build -f Dockerfile.win -t qtwin .`;
}if($ARGV[0] eq "buildwin"){
	print `podman build -f Dockerfile.win -t qtwin .`;
}if($ARGV[0] eq "buildlin"){
	print `podman build -f Dockerfile.linux -t qtlin .`;
}elsif($ARGV[0] eq "lin"){
	print `podman run -u=builder:builder --volume /mnt/files/scripts/CircuitTester:/CircuitTester:z -ti qtlin /bin/build`;
}elsif($ARGV[0] eq "test"){
	print `podman run -u=builder:builder --volume /mnt/files/scripts/CircuitTester:/CircuitTester:z -ti qtlin /bin/Test`;
}elsif($ARGV[0] eq "win"){
	print `podman run -u=builder:builder --volume /mnt/files/scripts/CircuitTester:/CircuitTester:z -ti qtwin /bin/build`;
}
else{
	print "Usage: build.sh [build|buildwin|buildlin|win|lin|test]";
}

print "\n";
