--TEST--
indexed_array_test2() Basic test
--SKIPIF--
<?php
if (!extension_loaded('indexed_array')) {
	echo 'skip';
}
?>
--FILE--
<?php
var_dump(indexed_array_test2());
var_dump(indexed_array_test2('PHP'));
?>
--EXPECT--
string(11) "Hello World"
string(9) "Hello PHP"
