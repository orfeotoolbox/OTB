#!/bin/bash
# Configure git for tar.xz
git config tar.tar.xz.command "xz -c"
# Create today's directory on serveur otb5-vm2
ssh otbpush@otb5-vm2.orfeo-toolbox.org mkdir -p /home/otbpush/test/$(date +%F)
# Delete latest
ssh otbpush@otb5-vm2.orfeo-toolbox.org rm -rf /home/otbpush/test/latest
# Create symilink
ssh otbpush@otb5-vm2.orfeo-toolbox.org ln -s /home/otbpush/test/$(date +%F) /home/otbpush/test/latest
# Push package
scp build_packages/*.run otbpush@otb5-vm2.orfeo-toolbox.org:/home/otbpush/test/$(date +%F)/.
# Push doc
scp build/{CookBook-*-html.tar.gz,\
/Documentation/{Cookbook/latex/CookBook-*.pdf,\
Documentation/Doxygen/OTB-Doxygen-*.tar.bz2}} \
otbpush@otb5-vm2.orfeo-toolbox.org:/home/otbpush/test/$(date +%F)/.

# Create zip, tar.gz and tar.xy source
git archive --format=zip -o OTB-develop-$CI_COMMIT_SHORT_SHA.zip HEAD
git archive --format=tgz -o OTB-develop-$CI_COMMIT_SHORT_SHA.tar.gz HEAD
git archive --format=tar.xz -o OTB-develop-$CI_COMMIT_SHORT_SHA.tar.xz HEAD
# Remove old source file
ssh otbpush@otb5-vm2.orfeo-toolbox.org \
rm /home/otbpush/test/$(date +%F)/OTB-develop-*.zip \
/home/otbpush/test/$(date +%F)/OTB-develop-*.tar.*

# Push new source file
scp OTB-develop-$CI_COMMIT_SHORT_SHA.* \
otbpush@otb5-vm2.orfeo-toolbox.org:/home/otbpush/test/$(date +%F)/

 