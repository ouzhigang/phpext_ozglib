<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('ozglib')) {
	dl('php_ozglib.' . PHP_SHLIB_SUFFIX);
}
$module = 'ozglib';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
