#!/usr/bin/perl
use strict;
use warnings;

sub aspect_ratio {
    my ($w, $h) = @_;
    return $w / $h;
}

sub parse_resolution {
    my ($res) = @_;
    if ($res =~ /^(\d+)x(\d+)$/) {
        return ($1, $2);
    }
    return;
}

my $inside_legacy = 0;
my $current_key = '';
my $collecting = 0;
my $buffer = '';
my $epsilon = 0.01;

while (<>) {
    chomp;
    s/\#.*//;            # Remove comments
    s/^\s+|\s+$//g;       # Trim whitespace
    next if $_ eq '';

    if (/^\s*"legacy_fisheye"\s*:\s*{\s*$/) {
        $inside_legacy = 1;
        next;
    }

    if ($inside_legacy) {
        if (/^\s*}\s*$/) {
            $inside_legacy = 0;
            next;
        }

        # Start of new resolution set
        if (/^\s*"(\d+x\d+)"\s*:\s*\[\s*$/) {
            $current_key = $1;
            next;
        }

        # End of resolution list
        if (/^\s*],?\s*$/) {
            $current_key = '';
            $buffer = '';
            $collecting = 0;
            next;
        }

        # Match lines like: ";res1,res2", or ",,,,res;"
        if (/^\s*"([^"]+)"\s*,?\s*$/) {
            my $line = $1;
            $buffer .= $line;

            # Process each complete segment delimited by ';'
            while ($buffer =~ /([^;]*);/) {
                my $segment = $1;
                $buffer =~ s/^[^;]*;//;  # Remove processed part

                $segment =~ s/^\s*,*//;  # Clean up leading commas
                $segment =~ s/\s+//g;    # Remove whitespace
                next if $segment eq '';

                my @res_list = split /,/, $segment;
                my $base_aspect;
                print "Validating in $current_key: [$segment]\n";

                for my $res (@res_list) {
                    next if $res eq '';
                    my ($w, $h) = parse_resolution($res);
                    unless (defined $w) {
                        warn "  Skipping invalid resolution '$res'\n";
                        next;
                    }

                    my $aspect = aspect_ratio($w, $h);
                    $base_aspect //= $aspect;

                    if (abs($aspect - $base_aspect) > $epsilon) {
                        printf "  ❌ Mismatch: %-10s got %.4f, expected ~%.4f\n", $res, $aspect, $base_aspect;
                    }
                }
            }
        }
    }
}

