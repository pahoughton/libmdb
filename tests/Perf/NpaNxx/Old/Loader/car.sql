use tdid_loc
go
select 
npa.nbr_npa, 
npa.nbr_nxx, 
convert( int, npa.idx_lat ), 
npa.nme_lcy, 
npa.cde_rgn, 
crgn.cat_rgn_rat, 
convert( char( 10 ), npa.dtx_eff_frm, 103 ), 
convert( int, npa.cde_tmx_zon_blc ), 
convert( int, npa.ind_dsv_blc ) 
from 
tdir_hom_npa npa, 
trgd_rat..tdir_rgn crgn 
where 
crgn.cde_rgn = npa.cde_rgn and 
crgn.cat_rgn_rat = 'CAR' 
go

