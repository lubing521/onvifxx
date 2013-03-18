#!/bin/sh

set -e

xml_dir=$(dirname $0)/xml

test -e stlvector.h && exit 0
cp /usr/share/gsoap/import/stlvector.h .

for f in $xml_dir/*.*
do
    test ${f##*.} != "xslt" && xsltproc $xml_dir/patch.xslt $f > ${f##*/}
done

wsdl_list=$(ls *.wsdl | tr "\n" " " | sed s/rw-2.wsdl// | sed s/bw-2.wsdl//)

mkdir src
wsdl2h -o ns1.h ws-addr.xsd
wsdl2h -o /dev/stdout $wsdl_list 2>/dev/null | soapcpp2 -inxCL -f100 -dsrc

sed -i 's/soap_namespaces/namespaces/' src/soap.nsmap
