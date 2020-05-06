using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Apis.Auth.OAuth2;
using Google.Apis.Drive.v3;
using Google.Apis.Services;
using Google.Apis.Download;
using Google.Apis.Upload;
using Google.Apis.Util.Store;
using System.IO;
using System.Threading;

namespace Editor.Module {
	using Properties;

	public class GoogleDrive {
		private string[] scopes = { DriveService.Scope.Drive };		//Googleドライブに許可を求める権限
		private DriveService service;
		public const string TagUnloadNode = "unload";       //Googleドライブのフォルダツリーで読み込まれていないノードのTag
		public const string RootFolderName = "root";       //Googleドライブにおけるルートフォルダー
		public event EventHandler GoogleServiceStarted;     //Googleサービスを起動し終えたときに発生するイベント
		public event EventHandler SetFolderListEnded;       //指定ノードの下に含まれるフォルダーを設定し終えたときに発生するイベント

		/// <summary>
		/// Googleドライブとの通信を開始します。
		/// </summary>
		public async void Start() {
			UserCredential credential;

			//ユーザーによる許可を求める
			using(var stream = new FileStream(Resources.Path_ClientToken, FileMode.Open, FileAccess.Read)) {
				string credPath = Common.AddToPathSplit(Path.GetTempPath()) + Resources.Path_Temp_GoogleCredential;
				credential = await GoogleWebAuthorizationBroker.AuthorizeAsync(
					GoogleClientSecrets.Load(stream).Secrets,
					scopes,
					"user",
					CancellationToken.None,
					new FileDataStore(credPath, true));
			}

			//DriveAPIのサービスを起動する
			service = new DriveService(new BaseClientService.Initializer() {
				HttpClientInitializer = credential,
				ApplicationName = Resources.AppName,
			});
			GoogleServiceStarted.Invoke(null, null);
		}

		/// <summary>
		/// 指定ノードの下に含まれるフォルダーを付け足します。
		/// </summary>
		/// <param name="node">親フォルダー</param>
		/// <param name="pageSize">数値を小さくするほど取得時間が早くなります</param>
		public async void SetFolderList(System.Windows.Forms.TreeNode node, int pageSize) {
			FilesResource.ListRequest listRequest = service.Files.List();
			listRequest.PageSize = pageSize;
			listRequest.PageToken = "";
			listRequest.OrderBy = "name";
			listRequest.Fields = "nextPageToken, files(name, id)";
			listRequest.Q = "'" + node.Tag.ToString() + "' in parents and mimeType contains 'folder'";

			node.Nodes.Clear();
			do {
				var result = await listRequest.ExecuteAsync();
				if(result.Files != null && result.Files.Count > 0) {
					foreach(var file in result.Files) {
						var subNode = new System.Windows.Forms.TreeNode();
						subNode.Text = file.Name;
						subNode.Tag = file.Id;

						//サブフォルダーがあるかどうかを調べる
						FilesResource.ListRequest sublistRequest = service.Files.List();
						sublistRequest.PageSize = 1;
						sublistRequest.PageToken = "";
						sublistRequest.OrderBy = "name";
						sublistRequest.Fields = "files(name)";
						sublistRequest.Q = "'" + file.Id + "' in parents and mimeType contains 'folder'";

						var subresult = await sublistRequest.ExecuteAsync();
						if(subresult.Files != null && subresult.Files.Count > 0) {
							var tempNode = new System.Windows.Forms.TreeNode();
							tempNode.Text = "";
							tempNode.Tag = TagUnloadNode;
							subNode.Nodes.Add(tempNode);        //まだ読み込まれていないサブフォルダー情報がある
						}

						node.Nodes.Add(subNode);
					}
				} else {
					return;
				}
				listRequest.PageToken = result.NextPageToken;       //次のページへ移る
			} while(listRequest.PageToken != null);
			SetFolderListEnded.Invoke(null, null);
		}

		public bool UploadFile() {

			return true;
		}

		public bool DownloadFile() {

			return true;
		}

		public bool DeleteFile() {

			return true;
		}

		public bool CreateFile() {

			return true;
		}
	}
}
