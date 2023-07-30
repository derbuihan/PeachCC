#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./peachcc "$input" > tmp.s || exit
  gcc -static -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 '0;'
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 10 "2+2*4;"
assert 4 "(3+5)/2;"
assert 10 "-10+20;"
assert 10 "--10;"
assert 10 "--+10;"

assert 0 '0==1;'
assert 1 '42==42;'
assert 1 '0!=1;'
assert 0 '42!=42;'

assert 1 '0<1;'
assert 0 '1<1;'
assert 0 '2<1;'
assert 1 '0<=1;'
assert 1 '1<=1;'
assert 0 '2<=1;'

assert 1 '1>0;'
assert 0 '1>1;'
assert 0 '1>2;'
assert 1 '1>=0;'
assert 1 '1>=1;'
assert 0 '1>=2;'

assert 3 '1; 2; 3;'

assert 3 'a=3; a;'
assert 8 'a=3; z=5; a+z;'
assert 6 'a=b=3; a+b;'
assert 96 'a=1; b=2; c=3; d=4; e=5; f=6; g=7; h=8; i=9; j=10; j+i*h+(g+f)/(e-d)+c-b*a;'

echo OK
