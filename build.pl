#!/bin/perl

if($ARGV[0] eq "build"){
	print `podman build -f Dockerfile.linux -t docker.io/radams15/qt-lin .`;
	print `podman build -f Dockerfile.win -t docker.io/radams15/qt-win .`;
}if($ARGV[0] eq "buildwin"){
	print `podman build -f Dockerfile.win -t docker.io/radams15/qt-win .`;
}if($ARGV[0] eq "buildlin"){
	print `podman build -f Dockerfile.linux -t docker.io/radams15/qt-lin .`;
}elsif($ARGV[0] eq "lin"){
	print `podman run -u=builder:builder --volume /mnt/files/scripts/CircuitTester:/CircuitTester:z -ti docker.io/radams15/qt-lin:latest /bin/build`;
}elsif($ARGV[0] eq "test"){
	print `podman run -u=builder:builder --volume /mnt/files/scripts/CircuitTester:/CircuitTester:z -ti docker.io/radams15/qt-lin:latest /bin/Test`;
}elsif($ARGV[0] eq "win"){
	print `podman run -u=builder:builder --volume /mnt/files/scripts/CircuitTester:/CircuitTester:z -ti docker.io/radams15/qt-win:latest /bin/build`;
}
else{
	print "Usage: build.sh [build|buildwin|buildlin|win|lin|test]";
}

print "\n";
