#!/usr/bin/perl

my $IN = @ARGV[0] || "deps.win";
my $OUT = @ARGV[1] ||"winbuild5/src/main";

sub find{
	my ($file, $where) = @_;

	my $command = "find $where -name $file";

	my $out = `$command`;

	if(!($out eq "")){
		return $out;
	}
}

open(FH, "<", $IN);

while(<FH>){
	my $data = $_;
	if($data =~ /(.*) (.*)/g){
		my ($file, $dst) = ($1, $2);

		my $src;

		if(substr($file, 0, 1) eq "/"){
			$src = $file;
		}else{
			$src = find $file, "'/usr/x86_64-w64-mingw32/sys-root/mingw/lib' '/usr/x86_64-w64-mingw32/sys-root/mingw/bin'";
		}

		if(! $src eq ""){
			if(! -d "$OUT/$dst"){
				`mkdir -p $OUT/$dst`;
			}

			`cp $src $OUT/$dst`;
		}
	}
}

close(FH);

`chmod -R 777 $OUT`;
