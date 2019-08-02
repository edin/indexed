--TEST--
indexed_array_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('indexed_array')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = indexed_array_test1();

var_dump($ret);
?>
--EXPECT--
The extension indexed_array is loaded and working!
NULL
