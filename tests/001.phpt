--TEST--
Check if indexed_array is loaded
--SKIPIF--
<?php
if (!extension_loaded('indexed_array')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "indexed_array" is available';
?>
--EXPECT--
The extension "indexed_array" is available
