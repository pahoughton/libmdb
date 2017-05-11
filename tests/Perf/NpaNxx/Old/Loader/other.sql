use tdid_loc
go
select 
npa.nbr_npa, 
npa.nbr_nxx, 
convert( int, npa.idx_lat ), 
npa.nme_lcy, 
npa.cde_rgn, 
crgn.cat_rgn_rat, 
cli.nbr_cor_vrt, 
cli.nbr_cor_hor, 
convert( char( 10 ), npa.dtx_eff_frm, 103 ), 
convert( int, npa.cde_tmx_zon_blc ), 
convert( int, npa.ind_dsv_blc ) 
from 
tdir_hom_npa npa, 
tdir_cli     cli, 
trgd_rat..tdir_rgn crgn 
where 
npa.cli_swt_hom = cli.idx_cli_cde and 
crgn.cde_rgn = npa.cde_rgn 
go
