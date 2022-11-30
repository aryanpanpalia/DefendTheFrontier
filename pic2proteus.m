%PIC2PROTEUS(filename,width,height)
%Supports JPEG, GIF, TIFF, PNG
%filename: A string containing the filename (with extension) to be
%converted
%width,height are effectively the number of col pixels and row pixels 
%repsectively that the image will be scaled to, to display on the Proteus.  
%Max size is width=320 and height=240. 
%Function will write a new file with name[filename(noextension),'FEH.pic']
function [] = pic2proteus(filename,width,height)

if(width>320 | height>240)
    fprintf('Max Proteus size is 320x240\n\n');
    return; 
end

if(exist(filename)==0)
    fprintf('FILE DOES NOT EXIST\n\n');
    return;
end


[orig_pic,map,trans]=imread(filename);

if(ndims(orig_pic)<3)
    fprintf(['File type does not contain RGB Data.\n' ...
        'Try exporting from an image viewer to a newer ' ...
        'PNG or JPEG format.\n\n']);
    return;
end

scale_pic = imresize(orig_pic,[height,width]);
if(~isempty(trans))
    scale_trans = imresize(trans,[height,width]);
else
    scale_trans = ones(height,width);
end
picsize=size(scale_pic);

filenoext=split(filename,'.');%delimite filename by extension
fehfile=fopen([filenoext{1},'FEH.pic'],'w');
fprintf(fehfile,'%d %d\n',picsize(1),picsize(2));
for(i=1:picsize(1))
    for(j=1:picsize(2))
        if(scale_trans(i,j)~=0)
            color=uint64(scale_pic(i,j,1))*16^4+...
                uint64(scale_pic(i,j,2))*16^2+uint64(scale_pic(i,j,3));
        else
            color=-1;
        end
        fprintf(fehfile,'%d\n',color);
    end
end

fclose(fehfile);

end