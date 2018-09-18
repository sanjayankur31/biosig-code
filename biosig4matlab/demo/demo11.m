% Demo for EOG regression analysis 
%
% Copyright (C) 2018 Alois Schloegl <alois.schloegl@gmail.com>
% 
% Prerequisites: 
% - Matlab or Octave
% - toolboxes: signal, biosig, nan

% References: 
% [1] A. Schlögl, C. Keinrath, D. Zimmermann, R. Scherer, R. Leeb, G. Pfurtscheller.
%   A fully automated correction method of EOG artifacts in EEG recordings.
%   Clin.Neurophys. 2007 Jan;118(1):98-104. Epub 2006 Nov 7 Paper(pdf) 
%   available online: http://dx.doi.org/10.1016/j.clinph.2006.09.003
%   preprint: https://pub.ist.ac.at/~schloegl/publications/schloegl2007eog.pdf

% [2] A. Schlögl, A. Ziehe, K.-R. Müller (2009)
%   Automated ocular artifact removal: comparing regression and component-based methods
%   Nature Precedings PrePrint
%   available online: http://precedings.nature.com/documents/3446/version/1

if exist('OCTAVE_VERSION','builtin')
        % pkg load mexbiosig
        pkg load signal 
        pkg load nan 
end
if ~exist('fir1','file') || ~exist('fftfilt','file')
        fprintf(2,'error: signal processing toolbox is missing')
end;        
if ~exist('regress_eog','file') || ~exist('mexSLOAD','file')
        fprintf(2,'error: toolbox "biosig for matlab" is missing, its available from http://biosig.sourceforge.net/download.html')
end;        

% load data     
[data,HDR]=mexSLOAD('PRIME_266.cnt');

% identify EEG and EOG channels 
eegchan=1:HDR.NS; 
eogchan=find(strcmp(HDR.Label,'VEOG'));
eegchan(eogchan)=[];

% filter for typical eog range (1-6Hz) in order to reduce other (non-EEG and non-EOG) 
% noise sources and get more accurate correction coefficients
B = fir1(HDR.SampleRate,[1 6]*2/HDR.SampleRate);
x = fftfilt(B,data);

% estimate correction coefficients 
R = regress_eog(x, eegchan, eogchan);

% correct for EOG artifcts
data_corrected = data * R.r0; 

% display result 
subplot(211)
plot([1:size(data,1)]'/HDR.SampleRate, [data(:,eegchan)+500, data_corrected(:,eegchan)-500, data(:,eogchan)/2])
set(gca,'xlim',[0,150])
subplot(212)
plot([1:size(data,1)]'/HDR.SampleRate, [data(:,eegchan)+500, data_corrected(:,eegchan)-500, data(:,eogchan)/2])
set(gca,'xlim',[30,40])


