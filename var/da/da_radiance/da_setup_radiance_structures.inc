subroutine da_setup_radiance_structures( grid, ob, iv )

   !---------------------------------------------------------------------------
   ! Purpose: Define, allocate and read of tovs raidance observation structure.
   !---------------------------------------------------------------------------

   implicit none

   type (domain) , intent(inout)   :: grid       ! model data
   type ( y_type), intent(inout)   :: ob         ! Observation structure.
   type (iv_type), intent(inout)   :: iv         ! O-B structure.

   character(len=200)          :: filename
   integer                     :: i, j, n, ios, ifgat
   logical                     :: lprinttovs 

   ! crtm_cloud
   integer  :: n1,n2,k,its,ite,jts,jte,kts,kte,inst
   integer  :: data_format, iret ! AHI radiance related
   integer  :: ahi_info_unit
   
   if (trace_use) call da_trace_entry("da_setup_radiance_structures")

   !-------------------------------------------------------------------
   ! [1.0] Initialize RTTOV coefs and innovations vector for radiance
   !-------------------------------------------------------------------

    call da_radiance_init(iv, ob)
    
    do n = 1, rtminit_nsensor
       iv%instid(n)%rad_monitoring = rad_monitoring(n)
    enddo

   !-------------------------------
   ! 1.1 Make thinning grids
   !------------------------------

   if (thinning) then
      allocate(thinning_grid(iv%num_inst,num_fgat_time))
      do ifgat=1,num_fgat_time
         do n=1,iv%num_inst
            call makegrids (n,thinning_mesh(n),ifgat)
         end do
      end do 
   end if

   !-------------------------------------------------------------------
   ! [2.0] Read NCEP bufr tovs data in radiance innovations vector
   !-------------------------------------------------------------------

   if ( (.not. use_filtered_rad) .and. (.not. use_pseudo_rad) .and. (.not. use_simulated_rad) ) then

      if (use_hirs2obs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from hirs2.bufr'
         filename = 'hirs2 '
         call da_read_obs_bufrtovs ('hirs2', iv, filename)
      end if

      if (use_msuobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from msu.bufr'
         filename = 'msu'
         call da_read_obs_bufrtovs ('msu  ', iv, filename)
      end if

      if (use_hirs3obs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from hirs3.bufr'
         filename = 'hirs3'
         call da_read_obs_bufrtovs('hirs3', iv, filename)
      end if

      if (use_amsuaobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from amsua.bufr'
         filename = 'amsua'
         call da_read_obs_bufrtovs ('amsua', iv, filename)
      end if

      if (use_amsubobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from amsub.bufr'
         filename = 'amsub'
         call da_read_obs_bufrtovs ('amsub', iv, filename)
      end if

      if (use_hirs4obs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from hirs4.bufr'
         filename = 'hirs4'
         call da_read_obs_bufrtovs('hirs4', iv, filename)
      end if

      if (use_mhsobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from mhs.bufr'
         filename = 'mhs'
         call da_read_obs_bufrtovs('mhs  ', iv, filename)
      end if

      if (use_mwtsobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from mwtsa.dat and mwtsb.dat'
         filename = 'mwtsa'
         call da_read_obs_fy3('mwts ', iv, filename)
         filename = 'mwtsb'
         call da_read_obs_fy3('mwts ', iv, filename)
      end if

      if (use_mwhsobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from mwhsa.dat and mwhsb.dat'
         filename = 'mwhsa'
         call da_read_obs_fy3('mwhs ', iv, filename)
         filename = 'mwhsb'
         call da_read_obs_fy3('mwhs ', iv, filename)
      end if

      if (use_mwhs2obs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from mwhs2.hdf'
         filename = 'mwhs2'
         call da_read_obs_hdf5mwhs2(iv, filename)
      end if

      if (use_atmsobs) then
         write(unit=stdout,fmt='(a)') 'Reading radiance 1b data from atms.bufr'
         filename = 'atms'
         call da_read_obs_bufratms('atms ', iv, filename)
      end if

      if (use_airsobs) then
         write(unit=stdout,fmt='(a)') 'Reading airs 1b data from airs.bufr'
         filename = 'airs'
         call da_read_obs_bufrairs ('airs     ',iv, filename)
      end if

      if (use_eos_amsuaobs) then
         write(unit=stdout,fmt='(a)') 'Reading eos_amsua 1b data from airs.bufr'
         filename = 'airs'
         call da_read_obs_bufrairs ('eos_amsua',iv, filename)
      end if

      if (use_hsbobs) then
         write(unit=stdout,fmt='(a)') 'Reading hsb 1b data from airs.bufr'
         filename = 'airs'
         call da_read_obs_bufrairs ('hsb      ',iv, filename)
      end if

      if (use_ssmisobs) then
         write(unit=stdout,fmt='(a)') 'Reading ssmis data from ssmis.bufr'
         filename = 'ssmis'
         call da_read_obs_bufrssmis ('ssmis    ',iv, filename)
      end if
      if (use_iasiobs) then
         write(unit=stdout,fmt='(a)') 'Reading iasi data from iasi.bufr'
         filename = 'iasi'
         call da_read_obs_bufriasi ('iasi     ',iv, filename)
      end if
      if (use_seviriobs) then
         write(unit=stdout,fmt='(a)') 'Reading seviri data from seviri.bufr'
         filename = 'seviri'
         call da_read_obs_bufrseviri ('seviri   ',iv, filename)
      end if
      if (use_amsr2obs) then
#if defined(HDF5)
         write(unit=stdout,fmt='(a)') 'Reading AMSR2 data in HDF5 format'
         call da_read_obs_hdf5amsr2 (iv, 'L1SGRTBR', 'L2SGCLWLD')
#else
         message(1)='To read AMSR2 data, WRFDA must be compiled with HDF5'
         call da_error(__FILE__,__LINE__,message(1:1))
#endif
      end if
      if (use_ahiobs) then
         call da_get_unit(ahi_info_unit)
         open(unit=ahi_info_unit,file='ahi_info',status='old',iostat=iret)
         if(iret /= 0)then
             call da_error(__FILE__,__LINE__,(/"Read ahi_info error: no such file"/))
         end if	  

        !read ahi information
         read(ahi_info_unit,*)  
         read(ahi_info_unit,*) data_format  
         close(ahi_info_unit)
         call da_free_unit(ahi_info_unit)

         if (data_format==1) then
#if defined(HDF5)
            write(unit=stdout,fmt='(a)') 'Reading AHI data from cma hdf5'
            call da_read_obs_hdf5ahi (iv, 'L1AHITBR', 'L2AHICLP')
#else			
            call da_error(__FILE__,__LINE__,(/"To read AHI data, WRFDA must be compiled with HDF5"/))
#endif	
         end if
         if (data_format==2) then		 
            write(unit=stdout,fmt='(a)') 'Reading AHI data from geocat NETCDF4'
            call da_read_obs_netcdf4ahi_geocat (iv, 'L1AHITBR', 'L2AHICLP')
         end if
         if (data_format==3) then		 
            write(unit=stdout,fmt='(a)') 'Reading AHI data from JAXA NETCDF4'
!            call da_read_obs_netcdf4ahi_jaxa (iv, 'L1AHITBR', 'L2AHICLP')
			call da_read_obs_netcdf4ahi_jaxa (iv, 'L1AHITBR', 'L1AHITBRP')
         end if		
         !if (data_format==4) then		 
            !filename = 'ahi'
            !call da_read_obs_bufrahi ('ahi   ',iv, filename)
         !end if	
      end if	  
      if (use_goesimgobs) then
         write(unit=stdout,fmt='(a)') 'Reading netcdf goes imager GVAR data'
         !if (imager_format==1)then
         !   write(unit=stdout,fmt='(a)') 'Reading goesimg data from HDF4 files'
         !   filename = 'HDF4IMAGER-13'
         !   call da_read_obs_hdf4imager(iv, filename)
         !   filename = 'HDF4IMAGER-15'
         !   call da_read_obs_hdf4imager(iv, filename)
         !end if
         !if (imager_format==2)then
         !   write(unit=stdout,fmt='(a)') 'Reading goesimg data from NETCDF files'
            filename = 'goes-13-imager'
            call da_read_obs_ncgoesimg(iv, filename)
            filename = 'goes-14-imager'
            call da_read_obs_ncgoesimg(iv, filename)
            filename = 'goes-15-imager'
            call da_read_obs_ncgoesimg(iv, filename)
         !end if
         !write(unit=stdout,fmt='(a)') 'Finish reading goesimg data'
      end if
      if (use_goesabiobs) then
         write(unit=stdout,fmt='(a)') 'Reading netcdf goes ABI radiance data'

         call da_read_obs_ncgoesabi(iv, 16)

         call da_read_obs_ncgoesabi(iv, 17)
      end if
      if (use_gmiobs) then
#if defined(HDF5)
         write(unit=stdout,fmt='(a)') 'Reading GMI data in HDF5 format'
         call da_read_obs_hdf5gmi (iv, '1B.GPM.GMI','2A.GPM.GMI')
#else
         message(1)='To read GMI data, WRFDA must be compiled with HDF5'
         call da_error(__FILE__,__LINE__,message(1:1))
#endif
      end if
   end if

   if ( use_filtered_rad ) then
      write(unit=stdout,fmt='(a)') 'Reading filtered radiance'
      call da_read_filtered_rad (iv)
   end if

   if ( use_simulated_rad ) then
      write(unit=stdout,fmt='(a)') 'Reading simulated radiance'
      call da_read_simulated_rad (iv)
   end if

   if ( use_pseudo_rad ) then
      write(unit=stdout,fmt='(a)') 'Reading pseudo radiance from namelist'    
      call da_read_pseudo_rad (iv)
   end if

   if (use_kma1dvar) then
      do i=1,rtminit_nsensor
         filename = ' '
         filename='kma1dvar-'//trim(iv%instid(i)%rttovid_string)//'.inv'
         write(unit=stdout,fmt='(a,a)')  ' Reading KMA 1dvar innovation from  ', filename
         call da_read_kma1dvar (i,iv, ob, filename)
      end do
   end if

   if (thinning) then
      do ifgat=1,num_fgat_time
           do n=1,iv%num_inst
              call destroygrids (n,ifgat)
           end do
      end do
      deallocate(thinning_grid)
   end if

   ! sorting obs into FGAT time bins

   call da_sort_rad(iv)

   !-----------------------------------------------------------------------------
   ! [3.0] create (smaller) ob structure:
   !-----------------------------------------------------------------------------

   if (.not. use_kma1dvar) then
      do i = 1,  ob % num_inst
         ob % instid(i) % num_rad = iv % instid(i) % num_rad
         if (ob % instid(i) % num_rad < 1) cycle
         allocate (ob % instid(i) % tb(ob % instid(i) % nchan,ob % instid(i)%num_rad))
         ob % instid(i) % tb(:,:) = iv % instid(i) % tb_inv(:,:)
      end do
   end if

! Calculate DT for Cloudy Radiance DA

   if (use_rad .and. crtm_cloud .and. .not. DT_cloud_model) then
      its = grid%xp % its
      ite = grid%xp % ite
      jts = grid%xp % jts
      jte = grid%xp % jte
      kts = grid%xp % kts
      kte = grid%xp % kte

      grid%xb%delt(its:ite,jts:jte,kts:kte) = 0.0

      do inst= 1, iv % num_inst
         do n=1,iv%instid(inst)%num_rad
	     i = int(iv%instid(inst)%info%i(1,n))
	     j = int(iv%instid(inst)%info%j(1,n))
   	     grid%xb%delt(i:i+1, j:j+1, kts:kte) = 1800.0
         end do
      end do
   endif

   if (trace_use) call da_trace_exit("da_setup_radiance_structures")

end subroutine da_setup_radiance_structures

