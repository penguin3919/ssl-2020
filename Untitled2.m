%result
c=[];
d=[];
%idx- check;
//a=zeros([34 200 160]);
e=zeros([34 1]);
f=1;

i=0;
k=1;

idx=kmeans(train3,34);

for i=1:2416
        c(idx(i)+34*e(idx(i)),:)=train3(i,:); 
        e(idx(i))=e(idx(i))+1;
        
        a(idx(i),k,:)=train3(i,:);

end


%{
b=zeros([192 1]);

idx2=kmeans(train4,192);


for i=1:1218
    if b(idx2(i))==0
        %d=[d;train4(i,:)];
        d(idx2(i),:)=train4(i,:);
        b(idx2(i))=1;
    end
end

fid=fopen('train10.txt','wt');

for i=1:383
fprintf(fid,'%3d,',c(i,:));
fprintf(fid,'\n');
end

fclose(fid);

fid=fopen('train11.txt','wt');

for i=1:192
fprintf(fid,'%3d,',d(i,:));
fprintf(fid,'\n');
end

fclose(fid);
%}
    