function [R,S] = regress_eog(D,ny,nx)
%  REGRESS_EOG yields the regression coefficients for 
%  correcting EOG artifacts in EEG recordings.
%  
%  The correction of a single record is obtained like this:      
%   [R,S2] = regress_eog(filename, el, ol)
%   [R,S2] = regress_eog(S1, el, ol)
%       
%  Correction matrix is obtained through
%   [R] = regress_eog(covm(S1,'E'), EL, OL)
%   S2 = S1 * R.ro;    % without offset correction
%   S2 = [ones(size(S1,1),1),S1] * R.r1;    % with offset correction
%  
%  S1   recorded data
%  EL   list of eeg channels: those channels will be corrected   
%  OL   eog/ecg channels. 
%       if OL is a vector, it represents the list of noise channels 
%       if OL is a matrix, OL references the noise channels 
%  R    rereferencing matrix for correction artifacts with regression analysis
%  S2   corrected EEG-signal      
%
% see also: SAVE2BKR
%
% Reference(s):
% [1] A. Schl�gl and G. Pfurtscheller,
%    EOG and ECG minimization based on regression analysis, Technical Report - SIESTA, 1997.
%    available online: http://www.dpmi.tu-graz.ac.at/%7Eschloegl/publications/Eog_min.pdf


% This program is free software; you can redistribute it and/or
% modify it under the terms of the GNU General Public License
% as published by the Free Software Foundation; either version 2
% of the License, or (at your option) any later version.
% 
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with this program; if not, write to the Free Software
% Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

%	$Revision: 1.5 $
%	$Id: regress_eog.m,v 1.5 2005-01-04 11:04:13 schloegl Exp $
%	(C) 1997-2005 by Alois Schloegl <a.schloegl@ieee.org>	
%    	This is part of the BIOSIG-toolbox http://biosig.sf.net/


if ischar(D),
        [D,H] = sload(D);
        C = covm(D,'E');
        
elseif size(D,1)==size(D,2)
        C = D; 
        H.NS = size(C,2)-1;
else
        H.NS = size(D,2);
        C = covm(D,'E');
end

R.datatype = 'ArtifactCorrection_Regression';
R.signalchannel = ny;
R.noise_channel = nx;
R.Mode = 1; % correction of the mean 
R.Mode = 0; % correction without mean 

a = speye(H.NS+1);
if size(nx,1)==1,  % list of noise channel 
        nx  = nx(:);
        
else    % noise channels are defined through rereferencing (e.g. bipoloar channels)
        tmp = H.NS+(1:size(nx,2)); 
        a(2:size(nx,1)+1,tmp+1) = nx;
        if rank(full(nx)) < size(nx,2),
                fprintf(2,'Warning REGRESS_EOG: referencing matrix is singular! \n');
        end; 
        C   = a'*C*a; 
        nx  = tmp';
        
end;
ny = ny(:);

r0 = speye(H.NS);
r1 = sparse(2:H.NS+1,1:H.NS,1);

b0 = -inv(C([1; nx+1], [1; nx+1])) * C([1; nx+1], ny+1);
r0(nx,ny) = b0(2:end,:);       % rearrange channel order
r1([1;1+nx], ny) = b0;       % rearrange channel order

R.r0 = a(2:end,2:end)*r0;
R.r1 = a*r1;

if size(D,1)==size(D,2),
        % R = R;         
        
elseif (nargout>1)
        % S = D * R.r0;   % without offset correction 
        S = [ones(size(D,1),1),D] * R.r1; % with offset correction
end

